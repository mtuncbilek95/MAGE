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
	class VImageView;

	struct ImageProps final
	{
		VkExtent3D imageSize = { 512, 512, 1 };
		u32 mipLevels = 1;
		u32 arrayLayers = 1;
		VkImageType imageType = VK_IMAGE_TYPE_2D;
		VkFormat imageFormat = VK_FORMAT_UNDEFINED;
		VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	};

	struct ViewProps final
	{
		VkImageAspectFlags aspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;
		u32 baseMipLevel = 0;
		u32 baseArrayLayer = 0;
	};

	class VImage final : public VkObject
	{
	public:
		VImage(const ImageProps& decs, VDevice* device);
		VImage(const ImageProps& desc, VkImage image, VDevice* device); // For swapchain images
		~VImage() override final;

		Owned<VImageView> CreateView(const ViewProps& desc);

		inline VkImage GetVkImage() const { return m_image; }

		inline const VkExtent3D& GetExtent() const { return m_props.imageSize; }
		inline u32 GetMipLevels() const { return m_props.mipLevels; }
		inline u32 GetArrayLayers() const { return m_props.arrayLayers; }
		inline VkImageType GetType() const { return m_props.imageType; }
		inline VkFormat GetFormat() const { return m_props.imageFormat; }
		inline VkImageUsageFlags GetUsage() const { return m_props.imageUsage; }

		void Destroy() override final;

	private:
		ImageProps m_props;

		VkImage m_image;
		b8 m_owned; // It will be false if image comes from swapchain.
	};
}