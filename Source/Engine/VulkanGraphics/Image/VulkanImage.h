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

	class VulkanImage final
	{
	public:
		VulkanImage(const ImageProps& desc, VulkanDevice* device);
		~VulkanImage();

		VulkanImageView CreateView(const ImageViewProps& desc);

		VkImage GetImage() const { return m_image; }
		VkDeviceMemory GetImageMemory() const { return m_imageMemory; }

		Math::Vec3u GetImageSize() const { return m_imageSize; }
		u32 GetMipLevels() const { return m_mipLevels; }
		u32 GetArrayLayers() const { return m_arrayLayers; }
		VkImageType GetImageType() const { return m_imageType; }
		VkFormat GetFormat() const { return m_format; }
		VkImageUsageFlags GetImageUsage() const { return m_imageUsage; }

	private:
		Math::Vec3u m_imageSize;
		u32 m_mipLevels;
		u32 m_arrayLayers;
		VkImageType m_imageType;
		VkFormat m_format;
		VkImageUsageFlags m_imageUsage;

		VkImage m_image;
		VkDeviceMemory m_imageMemory;

		VkDevice m_device;
		VkPhysicalDevice m_physicalDevice;

		VulkanDevice* m_deviceRef;
	};
}
