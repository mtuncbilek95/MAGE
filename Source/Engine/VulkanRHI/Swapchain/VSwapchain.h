/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"

namespace MAGE
{
	class VQueue;
	class VImage;
	class VImageView;
	class VFramebuffer;
	class VSemaphore;
	class VFence;
	class VRenderPass;

	struct SwapchainProps final
	{
		VkFormat format;
		VkPresentModeKHR presentMode;
		Math::Vec2u imageSize;
		u8 imageCount;
		VQueue* graphicsQueue;
	};

	class VSwapchain final : public VObject
	{
	public:
		VSwapchain(const SwapchainProps& desc, VDevice* device);
		~VSwapchain() override final;

		inline VkSwapchainKHR GetSwapchain() const { return m_swapchain; }
		inline VkSurfaceKHR GetSurface() const { return m_surface; }
		inline VkFormat GetFormat() const { return m_props.format; }
		inline VkPresentModeKHR GetPresentMode() const { return m_props.presentMode; }
		inline Math::Vec2u GetImageSize() const { return m_props.imageSize; }
		inline u8 GetImageCount() const { return m_props.imageCount; }

		VImage* GetImage(u32 index) const { return &*m_images[index]; }
		VImageView* GetImageView(u32 index) const { return &*m_imageViews[index]; }
		VRenderPass* GetRenderPass() const { return &*m_renderPass; }
		VFramebuffer* GetFramebuffer(u32 index) const { return &*m_frameBuffers[index]; }
		VFramebuffer* GetFramebuffer() const { return &*m_frameBuffers[m_currentFrame]; }

		void Resize(Math::Vec2u newSize);
		u32 AcquireNextImage(VSemaphore* semaphore, VFence* fence);
		void Present(u32 imageIndex, VSemaphore* semaphore);

		void Destroy() override final;

	private:
		SwapchainProps m_props; // Swapchain properties

		VkSwapchainKHR m_swapchain;
		VkSurfaceKHR m_surface;

		Vector<Owned<VImage>> m_images;
		Vector<Owned<VImageView>> m_imageViews;
		Vector<Owned<VFramebuffer>> m_frameBuffers;
		Owned<VRenderPass> m_renderPass;

		VkCommandPool m_resizePool;
		VkCommandBuffer m_resizeBuffer;

		u32 m_currentFrame = 0;
	};
}