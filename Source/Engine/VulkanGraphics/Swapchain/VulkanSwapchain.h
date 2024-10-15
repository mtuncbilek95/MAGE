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
	class IndWindow;
	class VulkanDevice;
	class VulkanQueue;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanImage;
	class VulkanImageView;
	class VulkanRenderPass;

	struct SwapchainProps final
	{
		VkFormat format;
		VkPresentModeKHR presentMode;
		Math::Vec2u imageSize;
		u8 imageCount;
		VulkanQueue* graphicsQueue;
		IndWindow* windowRef;
	};

	class VulkanSwapchain final
	{
	public:
		VulkanSwapchain(const SwapchainProps& desc, VulkanDevice* device);
		~VulkanSwapchain();

		void Resize(Math::Vec2u size);

		VkSwapchainKHR GetSwapchain() const { return m_swapchain; }
		VkSurfaceKHR GetSurface() const { return m_surface; }
		VkFormat GetFormat() const { return m_props.format; }
		VkPresentModeKHR GetPresentMode() const { return m_props.presentMode; }
		Math::Vec2u GetImageSize() const { return m_props.imageSize; }
		u8 GetImageCount() const { return m_props.imageCount; }

		VulkanImage* GetImage(u32 index) const { return &*m_images[index]; }
		VulkanImageView* GetImageView(u32 index) const { return &*m_imageViews[index]; }

		u32 AcquireNextImage(VulkanSemaphore* semaphore, VulkanFence* fence);
		void Present(u32 imageIndex, VulkanSemaphore* semaphore);

	private:
		SwapchainProps m_props; // Swapchain properties

		VkSwapchainKHR m_swapchain;
		VkSurfaceKHR m_surface;

		Vector<Owned<VulkanImage>> m_images;
		Vector<Owned<VulkanImageView>> m_imageViews;

		VkCommandPool m_resizePool;
		VkCommandBuffer m_resizeBuffer;

		VulkanDevice* m_deviceRef;
	};
}
