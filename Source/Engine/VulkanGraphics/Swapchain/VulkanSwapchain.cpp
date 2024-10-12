#include "VulkanSwapchain.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Queue/VulkanQueue.h"

#include "Engine/ErrorHandler/ErrorChecker.h"

#include "Engine/Window/WindowManager.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // DELUSION_WINDOWS

#if defined(DELUSION_LINUX)
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif // DELUSION_LINUX

#include <algorithm>

namespace MAGE
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainProps& desc, VulkanDevice* device) : m_device(device->GetDevice()), 
		m_instance(device->GetInstance()), m_physicalDevice(device->GetAdapter()), m_graphicsQueue(desc.graphicsQueue), 
		m_format(desc.format), m_presentMode(desc.presentMode), m_imageSize(desc.imageSize), m_imageCount(desc.imageCount), 
		m_swapchain(VK_NULL_HANDLE), m_surface(VK_NULL_HANDLE)
	{
		// Create command pool
		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = m_graphicsQueue->GetFamilyIndex();
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		ErrorUtils::VkAssert(vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_resizePool), "VulkanSwapchain");

		// Create command buffer
		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.commandPool = m_resizePool;
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_device, &commandBufferInfo, &m_resizeBuffer), "VulkanSwapchain");

		// Create surface
#if defined(DELUSION_WINDOWS)
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		surfaceCreateInfo.hwnd = Manager::Window::Get().GetWindow().GetNativeHandle();
		ErrorUtils::VkAssert(vkCreateWin32SurfaceKHR(device->GetInstance(), &surfaceCreateInfo, nullptr, &m_surface), "VulkanSwapchain");
#endif // DELUSION_WINDOWS

#if defined(DELUSION_LINUX)
		VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.connection = Manager::Window::Get().GetWindow().GetNativeConnection();
		surfaceCreateInfo.window = Manager::Window::Get().GetWindow().GetNativeHandle();
		ErrorUtils::VkAssert(vkCreateXcbSurfaceKHR(device->GetInstance(), &surfaceCreateInfo, nullptr, &m_surface), "VulkanSwapchain");
#endif // DELUSION_LINUX

		// Add resize callback
		Manager::Window::Get().GetWindow().FrameResizeCallback([this](Math::Vec2u size)
			{
				Resize(size);
			});

		// Initial resize call
		Resize(desc.imageSize);
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkFreeCommandBuffers(m_device, m_resizePool, 1, &m_resizeBuffer);
		vkDestroyCommandPool(m_device, m_resizePool, nullptr);

		m_images.clear();

		for (auto& imageView : m_imageViews)
			vkDestroyImageView(m_device, imageView, nullptr);

		if (m_swapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

		if (m_surface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	}

	void VulkanSwapchain::Resize(Math::Vec2u size)
	{
		if (size.x == 0 || size.y == 0)
			return;

		m_images.clear();

		for (auto& imageView : m_imageViews)
			vkDestroyImageView(m_device, imageView, nullptr);

		if (m_swapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities), "VulkanSwapchain");

		u32 formatCount;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr), "VulkanSwapchain");

		Vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, surfaceFormats.data()), "VulkanSwapchain");

		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == m_format && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				colorSpace = format.colorSpace;
				break;
			}
		}

		if (surfaceCapabilities.maxImageExtent.width > 0 && surfaceCapabilities.maxImageExtent.height > 0)
		{
			if (size.x > surfaceCapabilities.maxImageExtent.width || size.y > surfaceCapabilities.maxImageExtent.width)
				m_imageSize = { surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height };
			else
				m_imageSize = size;
		}

		// Check if requested present mode is supported
		u32 presentModeCount;
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr), "VulkanSwapchain");

		Vector<VkPresentModeKHR> presentModes(presentModeCount);
		ErrorUtils::VkAssert(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes.data()), "VulkanSwapchain");

		// If the requested present mode is not supported, use FIFO as default
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (std::find(presentModes.begin(), presentModes.end(), m_presentMode) != presentModes.end())
			presentMode = m_presentMode;

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = m_surface;
		swapchainCreateInfo.minImageCount = static_cast<u32>(m_imageCount);
		swapchainCreateInfo.imageFormat = m_format;
		swapchainCreateInfo.imageColorSpace = colorSpace;
		swapchainCreateInfo.imageExtent = { m_imageSize.x, m_imageSize.y };
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_FALSE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
		ErrorUtils::VkAssert(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain), "VulkanSwapchain");

		// Get swapchain images
		u32 imageCount;
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr), "VulkanSwapchain");

		// Resize image vector
		m_images.resize(imageCount);
		ErrorUtils::VkAssert(vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, m_images.data()), "VulkanSwapchain");

		// Create image views
		m_imageViews.resize(imageCount, VK_NULL_HANDLE);

		for (u32 i = 0; i < imageCount; ++i)
		{
			VkImageViewCreateInfo imageViewInfo = {};
			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.image = m_images[i];
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewInfo.format = m_format;
			imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewInfo.subresourceRange.baseMipLevel = 0;
			imageViewInfo.subresourceRange.levelCount = 1;
			imageViewInfo.subresourceRange.baseArrayLayer = 0;
			imageViewInfo.subresourceRange.layerCount = 1;

			ErrorUtils::VkAssert(vkCreateImageView(m_device, &imageViewInfo, nullptr, &m_imageViews[i]), "VulkanSwapchain");

			// CHANGE THE LAYOUT BEFORE START TO RENDER TO THE IMAGE, BE PREPARED FOR RENDERING

			// Transition image layout
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			ErrorUtils::VkAssert(vkBeginCommandBuffer(m_resizeBuffer, &beginInfo), "VulkanSwapchain");

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = m_images[i];
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier( m_resizeBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			ErrorUtils::VkAssert(vkEndCommandBuffer(m_resizeBuffer), "VulkanSwapchain");

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_resizeBuffer;

			ErrorUtils::VkAssert(vkQueueSubmit(m_graphicsQueue->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE), "VulkanSwapchain");
			ErrorUtils::VkAssert(vkQueueWaitIdle(m_graphicsQueue->GetQueue()), "VulkanSwapchain");
			ErrorUtils::VkAssert(vkResetCommandBuffer(m_resizeBuffer, 0), "VulkanSwapchain");
		}
	}

	u32 VulkanSwapchain::AcquireNextImage(VulkanSemaphore* semaphore, VulkanFence* fence)
	{
		VkSemaphore currentSemaphore = VK_NULL_HANDLE;
		VkFence currentFence = VK_NULL_HANDLE;

		u32 imageIndex = 0;
		ErrorUtils::VkAssert(vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, currentSemaphore, currentFence, &imageIndex), "VulkanSwapchain");

		return imageIndex;
	}

	void VulkanSwapchain::Present(u32 imageIndex, VulkanSemaphore* semaphore)
	{
		VkSemaphore currentSemaphore = VK_NULL_HANDLE;

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &currentSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &imageIndex;

		ErrorUtils::VkAssert(vkQueuePresentKHR(m_graphicsQueue->GetQueue(), &presentInfo), "VulkanSwapchain");
	}
}
