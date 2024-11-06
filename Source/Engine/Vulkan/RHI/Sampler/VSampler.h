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
	struct SamplerProps final
	{
		SamplerProps(vk::Filter magF, vk::Filter minF,
			vk::SamplerMipmapMode mipMode , vk::SamplerAddressMode addressU,
			vk::SamplerAddressMode addressV, vk::SamplerAddressMode addressW,
			f32 mipBias, b8 aEnable, f32 maxA, b8 cEnable,
			vk::CompareOp compOp, f32 minLod, f32 maxLod) : magFilter(magF), minFilter(minF), 
			mipmapMode(mipMode), addressModeU(addressU), addressModeV(addressV), addressModeW(addressW),
			mipLodBias(mipBias), anisotropyEnable(aEnable), maxAnisotropy(maxA), compareEnable(cEnable),
			compareOp(compOp), minLod(minLod), maxLod(maxLod)
		{}

		vk::Filter magFilter;
		vk::Filter minFilter;
		vk::SamplerMipmapMode mipmapMode;
		vk::SamplerAddressMode addressModeU;
		vk::SamplerAddressMode addressModeV;
		vk::SamplerAddressMode addressModeW;
		f32 mipLodBias;
		b8 anisotropyEnable;
		f32 maxAnisotropy;
		b8 compareEnable;
		vk::CompareOp compareOp;
		f32 minLod;
		f32 maxLod;
	};

	class VSampler final : public VkObject
	{
	public:
		VSampler(const SamplerProps& desc, VDevice* device);
		~VSampler() override final;

		inline vk::Sampler GetVkSampler() const { return m_sampler; }

		void Destroy() override final;

	private:
		SamplerProps m_props;

		vk::Sampler m_sampler;
	};
}
