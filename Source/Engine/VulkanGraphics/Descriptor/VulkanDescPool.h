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

namespace MAGE
{
	class VulkanDevice;

	struct DescPoolProps final
	{
		u32 maxSets;
		VkDescriptorPoolCreateFlags flags;
		Vector<VkDescriptorPoolSize> poolSizes;
	};

	class VulkanDescPool final
	{
	public:
		VulkanDescPool();
		VulkanDescPool(const DescPoolProps& desc, VulkanDevice* device);
		~VulkanDescPool();

		VkDescriptorPool GetPool() const { return m_pool; }
		VkDescriptorPoolCreateFlags GetFlags() const { return m_props.flags; }

	private:
		DescPoolProps m_props; // Descriptor pool properties

		VulkanDevice* m_deviceRef;
		VkDescriptorPool m_pool;
	};
}