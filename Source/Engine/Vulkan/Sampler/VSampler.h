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
	struct SamplerProps final
	{
		VkFilter magFilter = VK_FILTER_NEAREST;
		VkFilter minFilter = VK_FILTER_NEAREST;
		VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		f32 mipLodBias = 0.f;
		b8 anisotropyEnable = true;
		f32 maxAnisotropy = 16.f;
		b8 compareEnable = true;
		VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
		f32 minLod = 0.f;
		f32 maxLod = 1.f;
		VkBorderColor borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
	};

	class VSampler final : public VkObject
	{
	public:
		VSampler(const SamplerProps& desc, VDevice* device);
		~VSampler() override final;

		inline VkSampler GetVkSampler() const { return m_sampler; }

		void Destroy() override final;

	private:
		SamplerProps m_props;

		VkSampler m_sampler;
	};
}
