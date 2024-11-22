/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Vulkan/Core/VkObject.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class IndWindow;
	class VQueue;
	class VSemaphore;
	class VFence;
	class VImage;
	class VImageView;

	struct SwapchainProps final
	{
		Math::Vec2u imageSize = { 1600, 900 };
		u8 imageCount = 3;
#if defined(DELUSION_WINDOWS)
		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
#endif
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		VQueue* graphicsQueue = nullptr;
		IndWindow* usedWindow = nullptr;
	};

	class VSwapchain final : public VkObject
	{
	public:
		VSwapchain(const SwapchainProps& desc, VDevice* device);
		~VSwapchain() override final;

		VkSwapchainKHR GetSwapchain() const { return m_swapchain; }
		VkSurfaceKHR GetSurface() const { return m_surface; }
		VkFormat GetFormat() const { return m_props.format; }
		VkPresentModeKHR GetPresentMode() const { return m_props.presentMode; }
		Math::Vec2u GetImageSize() const { return m_props.imageSize; }
		u8 GetImageCount() const { return m_props.imageCount; }

		VImage* GetCurrentImage() const { return &*m_images[m_requestedIndex]; }
		VImageView* GetCurrentView() const { return &*m_views[m_requestedIndex]; }

		u32 AcquireNextImage(VSemaphore* semaphore, VFence* fence);
		void Present(const vector<VSemaphore*>& semaphore) const;
		void Resize(const Math::Vec2u& newSize);
		void Destroy() override final;

	private:
		SwapchainProps m_props;

		VkSwapchainKHR m_swapchain;
		VkSurfaceKHR m_surface;

		vector<Owned<VImage>> m_images;
		vector<Owned<VImageView>> m_views;

		VkFence m_barrierFence;
		VkCommandPool m_barrierPool;
		VkCommandBuffer m_barrierBuffer;

		u32 m_requestedIndex = 0;
	};
}
