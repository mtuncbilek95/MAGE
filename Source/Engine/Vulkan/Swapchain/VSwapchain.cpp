#include "VSwapchain.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"
#include "../Queue/VQueue.h"
#include "../Image/VImage.h"
#include "../Image/VImageView.h"
#include "../Sync/VFence.h"
#include "../Sync/VSemaphore.h"

#include "Engine/Window/IndWindow.h"

namespace MAGE
{
	VSwapchain::VSwapchain(const SwapchainProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_swapchain(VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo poolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		poolInfo.queueFamilyIndex = desc.graphicsQueue->GetFamilyIndex();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		ErrorUtils::VkAssert(vkCreateCommandPool(m_rootDevice->GetVkDevice(), &poolInfo, nullptr, &m_barrierPool), "VSwapchain");

		VkCommandBufferAllocateInfo bufferInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		bufferInfo.commandBufferCount = 1;
		bufferInfo.commandPool = m_barrierPool;
		bufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_rootDevice->GetVkDevice(), &bufferInfo, &m_barrierBuffer), "VSwapchain");

		VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		ErrorUtils::VkAssert(vkCreateFence(m_rootDevice->GetVkDevice(), &fenceInfo, nullptr, &m_barrierFence), "VSwapchain");

#if defined(DELUSION_WINDOWS)
		VkWin32SurfaceCreateInfoKHR surfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
		surfaceInfo.hinstance = GetModuleHandle(nullptr);
		surfaceInfo.hwnd = m_props.usedWindow->GetNativeHandle();
		ErrorUtils::VkAssert(vkCreateWin32SurfaceKHR(m_rootDevice->GetVkInstance(), &surfaceInfo, nullptr, &m_surface), "VSwapchain");
#endif

		Resize(desc.imageSize);
	}

	VSwapchain::~VSwapchain()
	{
		Destroy();
	}

	void VSwapchain::Resize(const Math::Vec2u& newSize)
	{
		if (newSize.x == 0 || newSize.y == 0)
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
			vkDestroySwapchainKHR(m_rootDevice->GetVkDevice(), m_swapchain, nullptr);

		VkSurfaceCapabilitiesKHR surfaceCap = {};
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_rootDevice->GetVkAdapter(), m_surface, &surfaceCap), "VSwapchain");

		u32 formatCount = 0;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_rootDevice->GetVkAdapter(), m_surface, &formatCount, nullptr), "VSwapchain");
		vector<VkSurfaceFormatKHR> formats(formatCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_rootDevice->GetVkAdapter(), m_surface, &formatCount, formats.data()), "VSwapchain");

		if (surfaceCap.maxImageExtent.width > 0 && surfaceCap.maxImageExtent.height > 0)
		{
			if (newSize.x > surfaceCap.maxImageExtent.width || newSize.y > surfaceCap.maxImageExtent.width)
				m_props.imageSize = { surfaceCap.maxImageExtent.width, surfaceCap.maxImageExtent.height };
			else
				m_props.imageSize = newSize;
		}

		u32 presentCount = 0;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_rootDevice->GetVkAdapter(), m_surface, &presentCount, nullptr), "VSwapchain");
		vector<VkPresentModeKHR> presentModes(presentCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_rootDevice->GetVkAdapter(), m_surface, &presentCount, presentModes.data()), "VSwapchain");

		if (std::find(presentModes.begin(), presentModes.end(), m_props.presentMode) != presentModes.end())
			m_props.presentMode = VK_PRESENT_MODE_FIFO_KHR;

		VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		createInfo.surface = m_surface;
		createInfo.minImageCount = static_cast<u32>(m_props.imageCount);
		createInfo.imageFormat = m_props.format;
		createInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		createInfo.imageExtent = { m_props.imageSize.x, m_props.imageSize.y };
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.preTransform = surfaceCap.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_props.presentMode;
		createInfo.clipped = VK_FALSE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		ErrorUtils::VkAssert(vkCreateSwapchainKHR(m_rootDevice->GetVkDevice(), &createInfo, nullptr, &m_swapchain), "VSwapchain");

		u32 imageCount;
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_rootDevice->GetVkDevice(), m_swapchain, &imageCount, nullptr), "VSwapchain");

		vector<VkImage> images(imageCount);
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_rootDevice->GetVkDevice(), m_swapchain, &imageCount, images.data()), "VSwapchain");

		for (u32 i = 0; i < imageCount; ++i)
		{
			ImageProps imageProps = {};
			imageProps.imageType = VK_IMAGE_TYPE_2D;
			imageProps.imageSize = { m_props.imageSize.x, m_props.imageSize.y, 1 };
			imageProps.mipLevels = 1;
			imageProps.arrayLayers = 1;
			imageProps.imageFormat = m_props.format;
			imageProps.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			m_images.push_back(MakeOwned<VImage>(imageProps, images[i], m_rootDevice));

			ViewProps viewProps = {};
			m_views.push_back(m_images[i]->CreateView(viewProps));

			VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			ErrorUtils::VkAssert(vkResetCommandBuffer(m_barrierBuffer, 0), "VSwapchain");
			ErrorUtils::VkAssert(vkBeginCommandBuffer(m_barrierBuffer, &beginInfo), "VSwapchain");

			VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			barrier.srcAccessMask = VK_ACCESS_NONE;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = m_images[i]->GetVkImage();
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier(m_barrierBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			ErrorUtils::VkAssert(vkEndCommandBuffer(m_barrierBuffer), "VSwapchain");

			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_barrierBuffer;

			ErrorUtils::VkAssert(vkQueueSubmit(m_props.graphicsQueue->GetVkQueue(), 1, &submitInfo, m_barrierFence), "VSwapchain");
			ErrorUtils::VkAssert(vkQueueWaitIdle(m_props.graphicsQueue->GetVkQueue()), "VSwapchain");

			vkWaitForFences(m_rootDevice->GetVkDevice(), 1, &m_barrierFence, VK_TRUE, u64_max);
			vkResetFences(m_rootDevice->GetVkDevice(), 1, &m_barrierFence);
		}
	}

	u32 VSwapchain::AcquireNextImage(VSemaphore* semaphore, VFence* fence)
	{
		ErrorUtils::VkAssert(vkAcquireNextImageKHR(m_rootDevice->GetVkDevice(), m_swapchain,
			UINT64_MAX, semaphore ? semaphore->GetVkSemaphore() : VK_NULL_HANDLE, fence ? fence->GetVkFence() : VK_NULL_HANDLE, &m_requestedIndex), "VulkanSwapchain");

		return m_requestedIndex;
	}

	void VSwapchain::Present(const vector<VSemaphore*>& waitSems) const
	{
		vector<VkSemaphore> sems(waitSems.size());

		for (u32 i = 0; i < sems.size(); i++)
			sems[i] = waitSems[i]->GetVkSemaphore();

		VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		presentInfo.waitSemaphoreCount = sems.size();
		presentInfo.pWaitSemaphores = sems.data();;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &m_requestedIndex;

		ErrorUtils::VkAssert(vkQueuePresentKHR(m_props.graphicsQueue->GetVkQueue(), &presentInfo), "VSwapchain");
	}

	void VSwapchain::Destroy()
	{
		if (m_barrierFence != VK_NULL_HANDLE)
		{
			vkDestroyFence(m_rootDevice->GetVkDevice(), m_barrierFence, nullptr);
			m_barrierFence = VK_NULL_HANDLE;
		}

		if (m_barrierBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(m_rootDevice->GetVkDevice(), m_barrierPool, 1, &m_barrierBuffer);
			m_barrierBuffer = VK_NULL_HANDLE;
		}

		if (m_barrierPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(m_rootDevice->GetVkDevice(), m_barrierPool, nullptr);
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
			vkDestroySwapchainKHR(m_rootDevice->GetVkDevice(), m_swapchain, nullptr);
			m_swapchain = VK_NULL_HANDLE;
		}

		if (m_surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(m_rootDevice->GetVkInstance(), m_surface, nullptr);
			m_surface = VK_NULL_HANDLE;
		}
	}
}
