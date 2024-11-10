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
	struct DescPoolProps final
	{
		Vector<vk::DescriptorPoolSize> poolSizes;
		u32 maxSets;
		vk::DescriptorPoolCreateFlags createFlag;
	};

	class VDescPool final : public VkObject
	{
	public:
		VDescPool(const DescPoolProps& desc, VDevice* device);
		~VDescPool() override final;

		inline vk::DescriptorPool GetVkPool() const { return m_pool; }

		void Destroy() override final;
	private:
		DescPoolProps m_props;

		vk::DescriptorPool m_pool;
	};
}
