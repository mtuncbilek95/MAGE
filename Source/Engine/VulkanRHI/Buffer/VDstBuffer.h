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
	struct DstBufferProps final
	{
		usize sizeInBytes;
		VkBufferUsageFlags usage;
	};

	class VDstBuffer final : public VObject
	{
	public:
		VDstBuffer(const DstBufferProps& desc, VDevice* device);
		~VDstBuffer() override final;

		inline VkBuffer GetBuffer() const { return m_buffer; }
		inline VkDeviceMemory GetMemory() const { return m_memory; }
		inline u64 GetTotalSize() const { return m_totalSize; }

		void MapMemory(RawBuffer buffer);
		void Destroy() override final;

	private:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkDeviceSize m_totalSize;

		DstBufferProps m_props;
	};
}
