#include "VulkanSwapchain.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Queue/VulkanQueue.h"
#include "../Image/VulkanImage.h"
#include "../Image/VulkanImageView.h"
#include "../Sync/VulkanSemaphore.h"
#include "../Sync/VulkanFence.h"
#include "../RenderPass/VulkanRenderPass.h"

#include "Engine/ErrorHandler/ErrorChecker.h"
#include "Engine/Window/WindowManager.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // DELUSION_WINDOWS

#include <algorithm>

namespace MAGE
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainProps& desc, VulkanDevice* device) : m_props(desc), m_deviceRef(device),
		m_surface(VK_NULL_HANDLE), m_swapchain(VK_NULL_HANDLE)
	{
		// Create command pool
		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = desc.graphicsQueue->GetFamilyIndex();
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		ErrorUtils::VkAssert(vkCreateCommandPool(device->GetDevice(), &commandPoolInfo, nullptr, &m_resizePool));

		// Create command buffer
		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.commandPool = m_resizePool;
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(device->GetDevice(), &commandBufferInfo, &m_resizeBuffer));

		// Create surface
#if defined(DELUSION_WINDOWS)
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		surfaceCreateInfo.hwnd = desc.windowRef->GetNativeHandle();
		ErrorUtils::VkAssert(vkCreateWin32SurfaceKHR(device->GetInstance(), &surfaceCreateInfo, nullptr, &m_surface));
#endif // DELUSION_WINDOWS

		Resize(desc.imageSize);
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkFreeCommandBuffers(m_deviceRef->GetDevice(), m_resizePool, 1, &m_resizeBuffer);
		vkDestroyCommandPool(m_deviceRef->GetDevice(), m_resizePool, nullptr);

		for (auto& image : m_images)
			image.reset();
		m_images.clear();
		m_images.shrink_to_fit();

		for (auto& imageView : m_imageViews)
			imageView.reset();
		m_imageViews.clear();
		m_imageViews.shrink_to_fit();

		if (m_swapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_deviceRef->GetDevice(), m_swapchain, nullptr);

		if (m_surface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(m_deviceRef->GetInstance(), m_surface, nullptr);
	}

	void VulkanSwapchain::Resize(Math::Vec2u size)
	{
		if (size.x == 0 || size.y == 0)
			return;

		for (auto& image : m_images)
			image.reset();
		m_images.clear();
		m_images.shrink_to_fit();

		for (auto& imageView : m_imageViews)
			imageView.reset();
		m_imageViews.clear();
		m_imageViews.shrink_to_fit();

		if (m_swapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_deviceRef->GetDevice(), m_swapchain, nullptr);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_deviceRef->GetAdapter(), m_surface, &surfaceCapabilities));

		u32 formatCount;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_deviceRef->GetAdapter(), m_surface, &formatCount, nullptr));

		Vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_deviceRef->GetAdapter(), m_surface, &formatCount, surfaceFormats.data()));

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
			if (size.x > surfaceCapabilities.maxImageExtent.width || size.y > surfaceCapabilities.maxImageExtent.width)
				m_props.imageSize = { surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height };
			else
				m_props.imageSize = size;
		}

		// Check if requested present mode is supported
		u32 presentModeCount;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_deviceRef->GetAdapter(), m_surface, &presentModeCount, nullptr));

		Vector<VkPresentModeKHR> presentModes(presentModeCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_deviceRef->GetAdapter(), m_surface, &presentModeCount, presentModes.data()));

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
		ErrorUtils::VkAssert(vkCreateSwapchainKHR(m_deviceRef->GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));

		// Get swapchain images
		u32 imageCount;
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_deviceRef->GetDevice(), m_swapchain, &imageCount, nullptr));

		Vector<VkImage> images(imageCount);
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_deviceRef->GetDevice(), m_swapchain, &imageCount, images.data()));

		for (u32 i = 0; i < imageCount; ++i)
		{
			ImageProps imageProps = {};
			imageProps.imageType = VK_IMAGE_TYPE_2D;
			imageProps.imageSize = { m_props.imageSize.x, m_props.imageSize.y, 1 };
			imageProps.mipLevels = 1;
			imageProps.arrayLayers = 1;
			imageProps.format = m_props.format;
			imageProps.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			m_images.push_back(MakeOwned<VulkanImage>(imageProps, images[i], m_deviceRef));

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

			ErrorUtils::VkAssert(vkBeginCommandBuffer(m_resizeBuffer, &beginInfo));

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

			ErrorUtils::VkAssert(vkEndCommandBuffer(m_resizeBuffer));

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_resizeBuffer;

			ErrorUtils::VkAssert(vkQueueSubmit(m_props.graphicsQueue->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE));
			ErrorUtils::VkAssert(vkQueueWaitIdle(m_props.graphicsQueue->GetQueue()));
			ErrorUtils::VkAssert(vkResetCommandBuffer(m_resizeBuffer, 0));
		}
	}

	u32 VulkanSwapchain::AcquireNextImage(VulkanSemaphore* semaphore, VulkanFence* fence)
	{
		u32 imageIndex = 0;
		vkAcquireNextImageKHR(m_deviceRef->GetDevice(), m_swapchain,
			UINT64_MAX, semaphore ? semaphore->GetSemaphore() : VK_NULL_HANDLE, fence ? fence->GetFence() : VK_NULL_HANDLE, &imageIndex);

		return imageIndex;
	}

	void VulkanSwapchain::Present(u32 imageIndex, VulkanSemaphore* semaphore)
	{
		VkSemaphore signalSemaphore = semaphore ? semaphore->GetSemaphore() : VK_NULL_HANDLE;
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = semaphore ? 1 : 0;
		presentInfo.pWaitSemaphores = &signalSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_props.graphicsQueue->GetQueue(), &presentInfo);
	}
}
