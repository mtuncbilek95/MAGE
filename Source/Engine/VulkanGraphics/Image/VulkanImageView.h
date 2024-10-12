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
	class VulkanImage;

	struct ImageViewProps final
	{
		VkImageViewType viewType;
		VkImageAspectFlags aspectFlags;
		u32 baseMipLevel;
		u32 baseArrayLayer;
	};

	class VulkanImageView final
	{
	public:
		VulkanImageView(const ImageViewProps& desc, VulkanImage* image, VulkanDevice* device);
		~VulkanImageView();

		VkImageView GetImageView() const { return m_imageView; }
		VkImageViewType GetViewType() const { return m_viewType; }
		VkImageAspectFlags GetAspectFlags() const { return m_aspectFlags; }

	private:
		VkImage m_image;
		VkImageView m_imageView;
		VkImageViewType m_viewType;
		VkImageAspectFlags m_aspectFlags;

		u32 m_baseMipLevel;
		u32 m_baseArrayLayer;

		VkDevice m_device;

		VulkanImage* m_imageRef;
		VulkanDevice* m_deviceRef;
	};
}
