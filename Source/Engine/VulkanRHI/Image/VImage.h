/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"
#include "../Image/VImageView.h"

namespace MAGE
{
	struct ImageProps final
	{
		Math::Vec3u imageSize;
		u32 mipLevels;
		u32 arrayLayers;
		VkImageType imageType;
		VkFormat format;
		VkImageUsageFlags imageUsage;
	};

	class VImage final : public VObject
	{
	public:
		VImage(const ImageProps& desc, VDevice* device);
		VImage(const ImageProps& desc, VkImage image, VDevice* device);	// For swapchain images
		~VImage() override final;

		Owned<VImageView> CreateView(const ImageViewProps& desc);

		inline VkImage GetImage() const { return m_image; }
		inline VkDeviceMemory GetMemory() const { return m_memory; }

		Math::Vec3u GetImageSize() const { return m_props.imageSize; }
		u32 GetMipLevels() const { return m_props.mipLevels; }
		u32 GetArrayLayers() const { return m_props.arrayLayers; }
		VkImageType GetImageType() const { return m_props.imageType; }
		VkFormat GetFormat() const { return m_props.format; }
		VkImageUsageFlags GetImageUsage() const { return m_props.imageUsage; }

		void Destroy() override final;

	private:
		ImageProps m_props;
		bool m_owned; // Checks if memory owned by swapchain or image itself

		VkImage m_image;
		VkDeviceMemory m_memory;
	};
}
