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

#include "VulkanImageView.h"

namespace MAGE
{
	class VulkanDevice;

	struct ImageProps final
	{
		Math::Vec3u imageSize;
		u32 mipLevels;
		u32 arrayLayers;
		VkImageType imageType;
		VkFormat format;
		VkImageUsageFlags imageUsage;
	};

	class VulkanImage final : public std::enable_shared_from_this<VulkanImage>
	{
	public:
		VulkanImage(const ImageProps& desc, VulkanDevice* device);
		VulkanImage(const ImageProps& desc, VkImage image, VulkanDevice* device);
		~VulkanImage();

		Owned<VulkanImageView> CreateView(const ImageViewProps& desc);

		VkImage GetImage() const { return m_image; }
		VkDeviceMemory GetImageMemory() const { return m_imageMemory; }

		Math::Vec3u GetImageSize() const { return m_props.imageSize; }
		u32 GetMipLevels() const { return m_props.mipLevels; }
		u32 GetArrayLayers() const { return m_props.arrayLayers; }
		VkImageType GetImageType() const { return m_props.imageType; }
		VkFormat GetFormat() const { return m_props.format; }
		VkImageUsageFlags GetImageUsage() const { return m_props.imageUsage; }

	private:
		ImageProps m_props;

		VkImage m_image;
		VkDeviceMemory m_imageMemory;

		VulkanDevice* m_deviceRef;
		bool m_ownedImage; // Swapchain images are not owned by this class
	};
}
