/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"
#include "Engine/Memory/RawBuffer.h"

namespace MAGE
{
	struct CombinedBufferProps final
	{
		usize sizeInBytes;
		VkBufferUsageFlags usage;
	};

	class VCombinedBuffer final : public VObject
	{
	public:
		VCombinedBuffer(const CombinedBufferProps& desc, VDevice* device);
		~VCombinedBuffer() override final;

		inline VkBuffer GetBuffer() const { return m_buffer; }
		inline VkDeviceMemory GetMemory() const { return m_memory; }
		inline u64 GetTotalSize() const { return m_totalSize; }
		inline u8* GetData() const { return m_data; }

		VkDeviceAddress GetDeviceAddress() const;

		void MapMemory(RawBuffer buffer);
		void Destroy() override final;

	private:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkDeviceSize m_totalSize;

		CombinedBufferProps m_props;

		u8* m_data;
	};
}
