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
	class VImageView;

	struct ImageProps final
	{
		ImageProps(vk::Extent3D size = { 512, 512, 1 }, u32 mips = 1, u32 arrays = 1,
			vk::ImageType type = vk::ImageType::e2D, vk::Format format = vk::Format::eR8G8B8A8Unorm , 
			vk::ImageUsageFlagBits usage = vk::ImageUsageFlagBits::eColorAttachment) : imageSize(size), 
			mipLevels(mips), arrayLayers(arrays), imageType(type), imageFormat(format), 
			imageUsage(usage) {}

		vk::Extent3D imageSize;
		u32 mipLevels;
		u32 arrayLayers;
		vk::ImageType imageType;
		vk::Format imageFormat;
		vk::ImageUsageFlagBits imageUsage;
	};

	struct ViewProps final
	{
		ViewProps(vk::ImageAspectFlagBits flag = vk::ImageAspectFlagBits::eColor, 
			u32 baseMip = 0, u32 baseArray = 0) : aspectFlag(flag), baseMipLevel(baseMip), 
			baseArrayLayer(baseArray) {}

		vk::ImageAspectFlagBits aspectFlag;
		u32 baseMipLevel;
		u32 baseArrayLayer;
	};

	class VImage final : public VkObject
	{
	public:
		VImage(const ImageProps& decs, VDevice* device);
		VImage(const ImageProps& desc, vk::Image image, VDevice* device); // For swapchain images
		~VImage() override final;

		Owned<VImageView> CreateView(const ViewProps& desc);

		inline vk::Image GetVkImage() const { return m_image; }

		inline const vk::Extent3D& GetExtent() const { return m_props.imageSize; }
		inline u32 GetMipLevels() const { return m_props.mipLevels; }
		inline u32 GetArrayLayers() const { return m_props.arrayLayers; }
		inline vk::ImageType GetType() const { return m_props.imageType; }
		inline vk::Format GetFormat() const { return m_props.imageFormat; }
		inline vk::ImageUsageFlagBits GetUsage() const { return m_props.imageUsage; }

		void Destroy() override final;

	private:
		vk::Image m_image;

		ImageProps m_props;
		b8 m_owned; // It will be false if image comes from swapchain.
	};
}