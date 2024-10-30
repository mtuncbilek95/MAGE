#include "VSwapchain.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Queue/VQueue.h"
#include "../Image/VImage.h"
#include "../Image/VImageView.h"
#include "../Framebuffer/VFramebuffer.h"
#include "../Sync/VSemaphore.h"
#include "../Sync/VFence.h"
#include "../RenderPass/VRenderPass.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif 

#include "Engine/Window/WindowManager.h"

namespace MAGE
{
	VSwapchain::VSwapchain(const SwapchainProps& desc, VDevice* device) : VObject(device),
		m_props(desc), m_swapchain(VK_NULL_HANDLE), m_surface(VK_NULL_HANDLE)
	{
		// Create command pool
		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = desc.graphicsQueue->GetFamilyIndex();
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		ErrorUtils::VkAssert(vkCreateCommandPool(m_rootDevice->GetDevice(), &commandPoolInfo, nullptr, &m_resizePool), "VSwapchain");

		// Create command buffer
		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.commandPool = m_resizePool;
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_rootDevice->GetDevice(), &commandBufferInfo, &m_resizeBuffer), "VSwapchain");

#if defined(DELUSION_WINDOWS)
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		surfaceCreateInfo.hwnd = Manager::Window::Get().GetWindow().GetNativeHandle();
		ErrorUtils::VkAssert(vkCreateWin32SurfaceKHR(m_rootDevice->GetInstance(), &surfaceCreateInfo, nullptr, &m_surface), "VSwapchain");
#endif // DELUSION_WINDOWS

		Resize(desc.imageSize);
	}

	VSwapchain::~VSwapchain()
	{
		Destroy();
	}

	void VSwapchain::Resize(Math::Vec2u newSize)
	{
		if (newSize.x == 0 || newSize.y == 0)
			return;

		for (auto& image : m_images)
			image->Destroy();
		m_images.clear();
		m_images.shrink_to_fit();

		for (auto& imageView : m_imageViews)
			imageView->Destroy();
		m_imageViews.clear();
		m_imageViews.shrink_to_fit();

		if (m_swapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_rootDevice->GetDevice(), m_swapchain, nullptr);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_rootDevice->GetAdapter(), m_surface, &surfaceCapabilities), "VSwapchain");

		u32 formatCount;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_rootDevice->GetAdapter(), m_surface, &formatCount, nullptr), "VSwapchain");

		Vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_rootDevice->GetAdapter(), m_surface, &formatCount, surfaceFormats.data()), "VSwapchain");

		// Check if requested format is supported
		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == m_props.format && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				colorSpace = format.colorSpace;
				break;
			}
		}

		// If the requested image size is not supported, use the max image extent
		if (surfaceCapabilities.maxImageExtent.width > 0 && surfaceCapabilities.maxImageExtent.height > 0)
		{
			if (newSize.x > surfaceCapabilities.maxImageExtent.width || newSize.y > surfaceCapabilities.maxImageExtent.width)
				m_props.imageSize = { surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height };
			else
				m_props.imageSize = newSize;
		}

		// Check if requested present mode is supported
		u32 presentModeCount;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_rootDevice->GetAdapter(), m_surface, &presentModeCount, nullptr), "VSwapchain");

		Vector<VkPresentModeKHR> presentModes(presentModeCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_rootDevice->GetAdapter(), m_surface, &presentModeCount, presentModes.data()), "VSwapchain");

		// If the requested present mode is not supported, use FIFO as default
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (std::find(presentModes.begin(), presentModes.end(), m_props.presentMode) != presentModes.end())
			presentMode = m_props.presentMode;

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = m_surface;
		swapchainCreateInfo.minImageCount = static_cast<u32>(m_props.imageCount);
		swapchainCreateInfo.imageFormat = m_props.format;
		swapchainCreateInfo.imageColorSpace = colorSpace;
		swapchainCreateInfo.imageExtent = { m_props.imageSize.x, m_props.imageSize.y };
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_FALSE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
		ErrorUtils::VkAssert(vkCreateSwapchainKHR(m_rootDevice->GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain), "VSwapchain");

		// Get swapchain images
		u32 imageCount;
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_rootDevice->GetDevice(), m_swapchain, &imageCount, nullptr), "VSwapchain");

		Vector<VkImage> images(imageCount);
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_rootDevice->GetDevice(), m_swapchain, &imageCount, images.data()), "VSwapchain");

		for (u32 i = 0; i < imageCount; ++i)
		{
			ImageProps imageProps = {};
			imageProps.imageType = VK_IMAGE_TYPE_2D;
			imageProps.imageSize = { m_props.imageSize.x, m_props.imageSize.y, 1 };
			imageProps.mipLevels = 1;
			imageProps.arrayLayers = 1;
			imageProps.format = m_props.format;
			imageProps.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			m_images.push_back(MakeOwned<VImage>(imageProps, images[i], m_rootDevice));

			ImageViewProps imageViewProps = {};
			imageViewProps.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewProps.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewProps.baseArrayLayer = 0;
			imageViewProps.baseMipLevel = 0;
			m_imageViews.push_back(m_images[i]->CreateView(imageViewProps));

			// CHANGE THE LAYOUT BEFORE START TO RENDER TO THE IMAGE, BE PREPARED FOR RENDERING

			// Transition image layout
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			ErrorUtils::VkAssert(vkResetCommandBuffer(m_resizeBuffer, 0), "VSwapchain");
			ErrorUtils::VkAssert(vkBeginCommandBuffer(m_resizeBuffer, &beginInfo), "VSwapchain");

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			barrier.srcAccessMask = VK_ACCESS_NONE;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = m_images[i]->GetImage();
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier(m_resizeBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			ErrorUtils::VkAssert(vkEndCommandBuffer(m_resizeBuffer), "VSwapchain");

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_resizeBuffer;

			ErrorUtils::VkAssert(vkQueueSubmit(m_props.graphicsQueue->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE), "VSwapchain");
			ErrorUtils::VkAssert(vkQueueWaitIdle(m_props.graphicsQueue->GetQueue()), "VSwapchain");
		}
	}

	u32 VSwapchain::AcquireNextImage(VSemaphore* semaphore, VFence* fence)
	{
		u32 imageIndex = 0;
		ErrorUtils::VkAssert(vkAcquireNextImageKHR(m_rootDevice->GetDevice(), m_swapchain,
			UINT64_MAX, semaphore ? semaphore->GetSemaphore() : VK_NULL_HANDLE, fence ? fence->GetFence() : VK_NULL_HANDLE, &imageIndex), "VSwapchain");

		m_currentFrame = imageIndex;
		return imageIndex;
	}

	void VSwapchain::Present(u32 imageIndex, VSemaphore* semaphore)
	{
		VkSemaphore signalSemaphore = semaphore ? semaphore->GetSemaphore() : VK_NULL_HANDLE;
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = semaphore ? 1 : 0;
		presentInfo.pWaitSemaphores = &signalSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &imageIndex;

		ErrorUtils::VkAssert(vkQueuePresentKHR(m_props.graphicsQueue->GetQueue(), &presentInfo), "VSwapchain");
	}

	void VSwapchain::Destroy()
	{
		if (m_resizeBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(m_rootDevice->GetDevice(), m_resizePool, 1, &m_resizeBuffer);
			m_resizeBuffer = VK_NULL_HANDLE;
		}

		if (m_resizePool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(m_rootDevice->GetDevice(), m_resizePool, nullptr);
			m_resizePool = VK_NULL_HANDLE;
		}

		for (auto& image : m_images)
			image.reset();
		m_images.clear();
		m_images.shrink_to_fit();

		for (auto& imageView : m_imageViews)
			imageView.reset();
		m_imageViews.clear();
		m_imageViews.shrink_to_fit();

		if (m_swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(m_rootDevice->GetDevice(), m_swapchain, nullptr);
			m_swapchain = VK_NULL_HANDLE;
		}

		if (m_surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(m_rootDevice->GetInstance(), m_surface, nullptr);
			m_surface = VK_NULL_HANDLE;
		}
	}
}
