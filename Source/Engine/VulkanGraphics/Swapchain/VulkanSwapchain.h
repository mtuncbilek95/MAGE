/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

namespace MAGE
{
	class VulkanDevice;
	class VulkanQueue;
	class VulkanSemaphore;
	class VulkanFence;

	struct SwapchainProps final
	{
		VkFormat format;
		VkPresentModeKHR presentMode;
		Math::Vec2u imageSize;
		u8 imageCount;
		VulkanQueue* graphicsQueue;
	};

	class VulkanSwapchain final
	{
	public:
		VulkanSwapchain(const SwapchainProps& desc, VulkanDevice* device);
		~VulkanSwapchain();

		void Resize(Math::Vec2u size);

		VkSwapchainKHR GetSwapchain() const { return m_swapchain; }
		VkSurfaceKHR GetSurface() const { return m_surface; }
		VkDevice GetDevice() const { return m_device; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }

		VkFormat GetFormat() const { return m_format; }
		VkPresentModeKHR GetPresentMode() const { return m_presentMode; }

		Math::Vec2u GetImageSize() const { return m_imageSize; }

		VkImage GetImage(u32 index) const { return m_images[index]; }
		VkImageView GetImageView(u32 index) const { return m_imageViews[index]; }

		u32 AcquireNextImage(VulkanSemaphore* semaphore, VulkanFence* fence);
		void Present(u32 imageIndex, VulkanSemaphore* semaphore);

	private:
		VkSwapchainKHR m_swapchain;
		VkSurfaceKHR m_surface;
		VkDevice m_device;
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;

		VkFormat m_format;
		VkPresentModeKHR m_presentMode;

		Math::Vec2u m_imageSize;
		u8 m_imageCount;

		Vector<VkImage> m_images;
		Vector<VkImageView> m_imageViews;

		VulkanQueue* m_graphicsQueue;

		VkCommandPool m_resizePool;
		VkCommandBuffer m_resizeBuffer;
	};
}
