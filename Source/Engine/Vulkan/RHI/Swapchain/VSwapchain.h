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
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	class VQueue;
	class VImage;
	class VImageView;
	class VSemaphore;
	class VFence;

	struct SwapchainProps final
	{
		SwapchainProps(vk::Format format = vk::Format::eR8G8B8A8Unorm,
			vk::PresentModeKHR mode = vk::PresentModeKHR::eFifoRelaxed,
			vk::Extent2D extent = { 1280, 720 }, u8 count = 2,
			VQueue* gQueue = nullptr) : imageFormat(format), presentMode(mode), 
			imageSize(extent), imageCount(count), graphicsQueue(gQueue) 
		{}

		vk::Format imageFormat;
		vk::PresentModeKHR presentMode;
		vk::Extent2D imageSize;
		u8 imageCount;
		VQueue* graphicsQueue;
	};

	class VSwapchain final : public VkObject
	{
	public:
		VSwapchain(const SwapchainProps& desc, VDevice* device);
		~VSwapchain() override final;

		void Resize(const vk::Extent2D& newSize);

		inline vk::SurfaceKHR GetVkSurface() const { return m_surface; }
		inline vk::SwapchainKHR GetVkSwapchain() const { return m_swapchain; }

		inline vk::Format GetVkFormat() const { return m_props.imageFormat; }
		inline vk::PresentModeKHR GetVkPresentMode() const { return m_props.presentMode; }
		inline const vk::Extent2D& GetImageSize() const { return m_props.imageSize; }
		inline u32 GetBufferCount() const { return m_props.imageCount; }

		VImage* GetSwapchainImage() const { return &*m_images[m_requestedIndex]; }
		VImageView* GetSwapchainView() const { return &*m_views[m_requestedIndex]; }

		u32 AcquireNextImage(VSemaphore* semaphore, VFence* fence);
		void Present(const Vector<VSemaphore*>& waitSems) const;
		void Destroy() override final;

	private:
		SwapchainProps m_props;

		vk::SurfaceKHR m_surface;
		vk::SwapchainKHR m_swapchain;

		Vector<Owned<VImage>> m_images;
		Vector<Owned<VImageView>> m_views;

		vk::Fence m_barrierFence;
		vk::CommandPool m_barrierPool;
		vk::CommandBuffer m_barrierBuffer;

		u32 m_requestedIndex = 0;
	};
}
