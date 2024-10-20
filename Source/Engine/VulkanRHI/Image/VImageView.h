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
	class VImage;

	struct ImageViewProps final
	{
		VkImageViewType viewType;
		VkImageAspectFlags aspectFlags;
		u32 baseMipLevel;
		u32 baseArrayLayer;
	};

	class VImageView final : public VObject
	{
	public:
		VImageView(const ImageViewProps& desc, VImage* image, VDevice* device);
		~VImageView() override final;

		inline VkImageView GetView() const { return m_view; }
		inline VkImageViewType GetViewType() const { return m_props.viewType; }
		inline VkImageAspectFlags GetAspectFlags() const { return m_props.aspectFlags; }

		Math::Vec3u GetImageSize() const;
		u32 GetMipLevels() const;
		u32 GetArrayLayers() const;
		VkImageType GetImageType() const;
		VkFormat GetFormat() const;
		VkImageUsageFlags GetImageUsage() const;

		void Destroy() override final;

	private:
		ImageViewProps m_props;
		VImage* m_imageRef;

		VkImageView m_view;
	};
}
