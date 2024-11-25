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
#include "Engine/Memory/RawBuffer.h"
#include "Engine/Vulkan/Memory/VMemory.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	struct BufferProps final
	{
		usize sizeInBytes;
		VkBufferUsageFlags usageFlags;
	};

	class VBuffer final : public VkObject
	{
	public:
		VBuffer(const BufferProps& desc, VDevice* device);
		~VBuffer() override final;

		inline VkBuffer GetVkBuffer() const { return m_buffer; }
		inline VkDeviceMemory GetVkMemory() const { return m_memory->m_memory; }
		inline VkBufferUsageFlags GetVkUsage() const { return m_props.usageFlags; }

		VkDeviceAddress GetVkAddress() const;

		inline usize GetMemoryOffset() const { return m_memoryOffset; }
		inline usize GetTotalSize() const { return m_props.sizeInBytes; }
		inline u8* GetMappedData() const { return m_memory->m_mappedData; }

		usize GetRequestedSize() const;

		void BindMemory(VMemory* memory);
		void Update(RawBuffer buffer, usize offset) const;
		void Destroy() override final;

	private:
		BufferProps m_props;
		VMemory* m_memory;

		VkBuffer m_buffer;
		usize m_memoryOffset;
	};
}
