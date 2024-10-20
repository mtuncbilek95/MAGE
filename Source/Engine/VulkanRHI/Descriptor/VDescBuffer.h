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
	class VDescLayout;

	class VDescBuffer final : VObject
	{
	public:
		VDescBuffer(VDescLayout* layout, VDevice* device);
		~VDescBuffer() override final;

		inline VkBuffer GetBuffer() const { return m_buffer; }
		inline VkDeviceMemory GetMemory() const { return m_memory; }

		inline u64 GetTotalSize() const { return m_totalSize; }
		inline u64 GetOffset() const { return m_offset; }

		void MapMemory(RawBuffer buffer, u64 offset);
		void Destroy() override final;

	private:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkDeviceSize m_totalSize;
		VkDeviceSize m_offset;

		VDescLayout* m_layoutRef;
	};
}