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

	struct DescLayoutBinding final
	{
		u32 binding;
		VkDescriptorType type;
		u32 count;
		VkShaderStageFlags stageFlags;
	};

	struct DescLayoutProps final
	{
		Vector<DescLayoutBinding> bindings;
		VkDescriptorSetLayoutCreateFlags initFlags;
	};

	class VulkanDescLayout final
	{
	public:
		VulkanDescLayout(const DescLayoutProps& desc, VulkanDevice* device);
		~VulkanDescLayout();

		VkDescriptorSetLayout GetLayout() const { return m_layout; }
		VkDescriptorSetLayoutCreateFlags GetFlags() const { return m_props.initFlags; }

	private:
		DescLayoutProps m_props; // Descriptor layout properties

		VulkanDevice* m_deviceRef;
		VkDescriptorSetLayout m_layout;
	};
}
