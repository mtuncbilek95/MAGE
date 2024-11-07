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
#include "Engine/Data/Memory/RawBuffer.h"
#include "Engine/Vulkan/RHI/Memory/VMemory.h"

#include <vulkan/vulkan.hpp>

namespace MAGE
{
	struct BufferProps final
	{
		BufferProps(usize size = 0, 
			vk::BufferUsageFlags flag = {},
			VMemory* mem = nullptr) : sizeInBytes(size), usageFlags(flag), memory(mem)
		{}

		usize sizeInBytes;
		vk::BufferUsageFlags usageFlags;
		VMemory* memory;
	};

	class VBuffer final : public VkObject
	{
	public:
		VBuffer(const BufferProps& desc, VDevice* device);
		~VBuffer() override final;

		inline vk::Buffer GetVkBuffer() const { return m_buffer; }
		inline vk::DeviceMemory GetVkMemory() const { return m_props.memory->m_memory; }
		inline vk::BufferUsageFlags GetVkUsage() const { return m_props.usageFlags; }
		
		vk::DeviceAddress GetVkAddress() const;
		
		inline usize GetMemoryOffset() const { return m_memoryOffset; }
		inline usize GetTotalSize() const { return m_props.sizeInBytes; }

		inline u8* GetMappedData() const { return m_props.memory->m_mappedData; }

		void Update(RawBuffer buffer) const;

		void Destroy() override final;

	private:
		BufferProps m_props;

		vk::Buffer m_buffer;
		usize m_memoryOffset;
	};
}