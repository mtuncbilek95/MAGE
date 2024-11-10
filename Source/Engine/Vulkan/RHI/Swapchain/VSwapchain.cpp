#include "VSwapchain.h"

#include "../Device/VDevice.h"
#include "../Image/VImage.h"
#include "../Image/VImageView.h"
#include "../Queue/VQueue.h"
#include "../Sync/VFence.h"
#include "../Sync/VSemaphore.h"

#include "Engine/Vulkan/Core/VkAssert.h"
#include "Engine/Window/WindowManager.h"

namespace MAGE
{
	VSwapchain::VSwapchain(const SwapchainProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::CommandPoolCreateInfo poolInfo = {};
		poolInfo.queueFamilyIndex = desc.graphicsQueue->GetFamilyIndex();
		poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createCommandPool(&poolInfo, nullptr, &m_barrierPool), "VSwapchain");

		vk::CommandBufferAllocateInfo bufferInfo = {};
		bufferInfo.commandBufferCount = 1;
		bufferInfo.commandPool = m_barrierPool;
		bufferInfo.level = vk::CommandBufferLevel::ePrimary;
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().allocateCommandBuffers(&bufferInfo, &m_barrierBuffer), "VSwapchain");

		vk::FenceCreateInfo fenceInfo = vk::FenceCreateInfo();
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createFence(&fenceInfo, nullptr, &m_barrierFence), "VSwapchain");

#if defined(DELUSION_WINDOWS)
		vk::Win32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.hinstance = GetModuleHandle(nullptr);
		surfaceInfo.hwnd = Manager::Window::Get().GetWindow().GetNativeHandle();
		ErrorUtils::VkAssert(m_rootDevice->GetVkInstance().createWin32SurfaceKHR(&surfaceInfo, nullptr, &m_surface), "VSwapchain");
#endif

		Resize(desc.imageSize);
	}

	VSwapchain::~VSwapchain()
	{
		Destroy();
	}

	void VSwapchain::Resize(const vk::Extent2D& newSize)
	{
		if (newSize.width == 0 || newSize.height == 0)
			return;

		for (auto& image : m_images)
			image->Destroy();
		m_images.clear();
		m_images.shrink_to_fit();

		for (auto& imageView : m_views)
			imageView->Destroy();
		m_views.clear();
		m_views.shrink_to_fit();

		if (m_swapchain != VK_NULL_HANDLE)
			m_rootDevice->GetVkDevice().destroySwapchainKHR(m_swapchain);

		vk::SurfaceCapabilitiesKHR surfaceCap = vk::SurfaceCapabilitiesKHR();
		ErrorUtils::VkAssert(m_rootDevice->GetVkAdapter().getSurfaceCapabilitiesKHR(m_surface, &surfaceCap), "VSwapchain");

		u32 formatCount = 0;
		ErrorUtils::VkAssert(m_rootDevice->GetVkAdapter().getSurfaceFormatsKHR(m_surface, &formatCount, nullptr), "VSwapchain");
		Vector<vk::SurfaceFormatKHR> formats(formatCount);
		ErrorUtils::VkAssert(m_rootDevice->GetVkAdapter().getSurfaceFormatsKHR(m_surface, &formatCount, formats.data()), "VSwapchain");

		if (surfaceCap.maxImageExtent.width > 0 && surfaceCap.maxImageExtent.height > 0)
		{
			if (newSize.width > surfaceCap.maxImageExtent.width || newSize.height > surfaceCap.maxImageExtent.width)
				m_props.imageSize = vk::Extent2D(surfaceCap.maxImageExtent.width, surfaceCap.maxImageExtent.height);
			else
				m_props.imageSize = newSize;
		}

		u32 presentCount = 0;
		ErrorUtils::VkAssert(m_rootDevice->GetVkAdapter().getSurfacePresentModesKHR(m_surface, &presentCount, nullptr), "VSwapchain");
		Vector<vk::PresentModeKHR> presentModes(presentCount);
		ErrorUtils::VkAssert(m_rootDevice->GetVkAdapter().getSurfacePresentModesKHR(m_surface, &presentCount, presentModes.data()), "VSwapchain");

		if (std::find(presentModes.begin(), presentModes.end(), m_props.presentMode) != presentModes.end())
			m_props.presentMode = vk::PresentModeKHR::eFifo;

		vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR();
		createInfo.surface = m_surface;
		createInfo.minImageCount = m_props.imageCount;
		createInfo.imageFormat = m_props.imageFormat;
		createInfo.imageExtent = m_props.imageSize;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		createInfo.presentMode = m_props.presentMode;
		createInfo.clipped = false;
		createInfo.oldSwapchain = nullptr;
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createSwapchainKHR(&createInfo, nullptr, &m_swapchain), "VSwapchain");

		u32 imageCount = 0;
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().getSwapchainImagesKHR(m_swapchain, &imageCount, nullptr), "VSwapchain");
		Vector<vk::Image> images(imageCount);
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().getSwapchainImagesKHR(m_swapchain, &imageCount, images.data()), "VSwapchain");

		for (u32 i = 0; i < imageCount; ++i)
		{
			ImageProps imageProps = ImageProps();
			imageProps.imageSize = vk::Extent3D(m_props.imageSize, 1);
			imageProps.imageFormat = m_props.imageFormat;

			m_images.push_back(MakeOwned<VImage>(imageProps, images[i], m_rootDevice));
			m_views.push_back(m_images[i]->CreateView(ViewProps()));

			m_barrierBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
			m_barrierBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

			const vk::ImageMemoryBarrier barrier({}, {}, vk::ImageLayout::eUndefined,
				vk::ImageLayout::ePresentSrcKHR, 0, 0, images[i], vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
			m_barrierBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, nullptr, nullptr, barrier);

			m_barrierBuffer.end();
			m_props.graphicsQueue->GetVkQueue().submit(vk::SubmitInfo(0, nullptr, nullptr, 1, &m_barrierBuffer), m_barrierFence);

			ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().waitForFences(1, &m_barrierFence, false, UINT64_MAX), "VSwapchain");
			ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().resetFences(1, &m_barrierFence), "VSwapchain");
		}
	}

	u32 VSwapchain::AcquireNextImage(VSemaphore* semaphore, VFence* fence)
	{
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().acquireNextImageKHR(m_swapchain, u64_max, 
			semaphore ? semaphore->GetVkSemaphore() : vk::Semaphore(), fence ? fence->GetVkFence() : vk::Fence(), &m_requestedIndex), "VSwapchain");

		return m_requestedIndex;
	}

	void VSwapchain::Present(const Vector<VSemaphore*>& waitSems) const
	{
		Vector<vk::Semaphore> waits(waitSems.size(), VK_NULL_HANDLE);

		for (u32 i = 0; i < waits.size(); i++)
			waits[i] = waitSems[i]->GetVkSemaphore();

		vk::Result returnResult = {};

		vk::PresentInfoKHR present = {};
		present.pImageIndices = &m_requestedIndex;
		present.swapchainCount = 1;
		present.pSwapchains = &m_swapchain;
		present.waitSemaphoreCount = waits.size();
		present.pWaitSemaphores = waits.data();
		present.pResults = &returnResult;

		ErrorUtils::VkAssert(m_props.graphicsQueue->GetVkQueue().presentKHR(&present), "VSwapchain");
		ErrorUtils::VkAssert(returnResult, "VSwapchain - Present");
	}

	void VSwapchain::Present(VSemaphore* waitSem) const
	{
		vk::Semaphore wait = waitSem->GetVkSemaphore();
		vk::Result returnResult = {};

		vk::PresentInfoKHR present = {};
		present.pImageIndices = &m_requestedIndex;
		present.swapchainCount = 1;
		present.pSwapchains = &m_swapchain;
		present.waitSemaphoreCount = 1;
		present.pWaitSemaphores = &wait;
		present.pResults = &returnResult;

		ErrorUtils::VkAssert(m_props.graphicsQueue->GetVkQueue().presentKHR(&present), "VSwapchain");
		ErrorUtils::VkAssert(returnResult, "VSwapchain - Present");
	}

	void VSwapchain::Destroy()
	{
		if (m_barrierFence != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyFence(m_barrierFence);
			m_barrierFence = VK_NULL_HANDLE;
		}

		if (m_barrierBuffer != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().freeCommandBuffers(m_barrierPool, 1, &m_barrierBuffer);
			m_barrierBuffer = VK_NULL_HANDLE;
		}

		if (m_barrierPool != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyCommandPool(m_barrierPool);
			m_barrierPool = VK_NULL_HANDLE;
		}

		for (auto& image : m_images)
			image->Destroy();
		m_images.clear();
		m_images.shrink_to_fit();

		for (auto& imageView : m_views)
			imageView->Destroy();
		m_views.clear();
		m_views.shrink_to_fit();

		if (m_swapchain != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroySwapchainKHR(m_swapchain);
			m_swapchain = VK_NULL_HANDLE;
		}

		if (m_surface != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkInstance().destroySurfaceKHR(m_surface);
			m_surface = VK_NULL_HANDLE;
		}
	}
}
