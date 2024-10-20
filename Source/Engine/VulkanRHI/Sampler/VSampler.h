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
	struct SamplerProps final
	{
		VkFilter magFilter;
		VkFilter minFilter;
		VkSamplerMipmapMode mipmapMode;
		VkSamplerAddressMode addressModeU;
		VkSamplerAddressMode addressModeV;
		VkSamplerAddressMode addressModeW;
		f32 mipLodBias;
		b8 anisotropyEnable;
		f32 maxAnisotropy;
		b8 compareEnable;
		VkCompareOp compareOp;
		f32 minLod;
		f32 maxLod;
		VkBorderColor borderColor;
	};

	class VSampler final : public VObject
	{
	public:
		VSampler(const SamplerProps& desc, VDevice* device);
		~VSampler() override final;

		inline VkSampler GetSampler() const { return m_sampler; }

		void Destroy() override final;

	private:
		SamplerProps m_props;

		VkSampler m_sampler;
	};
}
