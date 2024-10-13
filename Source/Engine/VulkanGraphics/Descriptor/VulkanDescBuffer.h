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

#include "VulkanDescLayout.h"

namespace MAGE
{
	class VulkanDevice;
	class VulkanDescLayout;

	class VulkanDescBuffer final
	{
	public:
		VulkanDescBuffer(VulkanDescLayout* layout, VulkanDevice* device);
		~VulkanDescBuffer();

		VkDescriptorSetLayout GetLayout() const { return m_layoutRef->GetLayout(); }
		VkBuffer GetBuffer() const { return m_buffer; }
		VkDeviceMemory GetMemory() const { return m_memory; }

		inline u64 GetTotalSize() const { return m_totalSize; }
		inline u64 GetOffset() const { return m_offset; }

	private:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkDeviceSize m_totalSize;
		VkDeviceSize m_offset;

		VulkanDescLayout* m_layoutRef;
		VulkanDevice* m_deviceRef;
	};
}
