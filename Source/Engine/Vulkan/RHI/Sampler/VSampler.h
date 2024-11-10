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
		SamplerProps(vk::Filter magF = vk::Filter::eNearest, vk::Filter minF = vk::Filter::eNearest,
			vk::SamplerMipmapMode mipMode = vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode addressU = vk::SamplerAddressMode::eRepeat,
			vk::SamplerAddressMode addressV = vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode addressW = vk::SamplerAddressMode::eRepeat,
			f32 mipBias = 0.0f, b8 aEnable = true, f32 maxA = 1.0f, b8 cEnable = true,
			vk::CompareOp compOp = vk::CompareOp::eAlways, f32 minLod = 0.0f, f32 maxLod = 1.0f) : magFilter(magF), minFilter(minF),
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
