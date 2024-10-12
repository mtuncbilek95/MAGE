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
	class VulkanDescLayout;

	class VulkanDescBuffer final
	{
	public:
		VulkanDescBuffer(VulkanDescLayout* layout, VulkanDevice* device);
		~VulkanDescBuffer();

		VkDescriptorSetLayout GetLayout() const { return m_layout; }
		VkBuffer GetBuffer() const { return m_buffer; }
		VkDeviceMemory GetMemory() const { return m_memory; }

		u64 GetTotalSize() const { return m_totalSize; }
		u64 GetOffset() const { return m_offset; }

	private:
		VkDescriptorSetLayout m_layout;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;

		VkDevice m_device;
		VkPhysicalDevice m_physicalDevice;

		VkDeviceSize m_totalSize;
		VkDeviceSize m_offset;
	};
}
