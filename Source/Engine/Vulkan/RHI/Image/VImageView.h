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
#include "Engine/Vulkan/RHI/Image/VImage.h"

#include <vulkan/vulkan.hpp>

namespace MAGE
{
	struct ImageViewProps final
	{
		ImageViewProps(VImage* owner = nullptr, vk::ImageViewType type = vk::ImageViewType::e2D,
			vk::ImageAspectFlagBits flag = vk::ImageAspectFlagBits::eColor,
			u32 baseMip = 0, u32 baseArray = 0) : ownerImage(owner) , viewType(type), aspectFlag(flag), 
			baseMipLevel(baseMip), baseArrayLayer(baseArray)
		{}

		VImage* ownerImage;
		vk::ImageViewType viewType;
		vk::ImageAspectFlagBits aspectFlag;
		u32 baseMipLevel;
		u32 baseArrayLayer;
	};

	class VImageView final : public VkObject
	{
	public:
		VImageView(const ImageViewProps& desc, VDevice* device);
		~VImageView() override final;

		inline vk::ImageView GetVkView() const { return m_view; }
		inline vk::Image GetVkImage() const { return m_props.ownerImage->GetVkImage(); }
		inline vk::Format GetFormat() const { return m_props.ownerImage->GetFormat(); }
		inline const vk::Extent3D& GetExtent() const { return m_props.ownerImage->GetExtent(); }

		inline VImage* GetImage() const { return m_props.ownerImage; }
		inline vk::ImageViewType GetType() const { return m_props.viewType; }
		inline vk::ImageAspectFlagBits GetAspect() const { return m_props.aspectFlag; }
		inline u32 GetBaseMipLevel() const { return m_props.baseMipLevel; }
		inline u32 GetBaseArrayLayer() const { return m_props.baseArrayLayer; }

		void Destroy() override final;

	private:
		vk::ImageView m_view;

		ImageViewProps m_props;
	};
}
