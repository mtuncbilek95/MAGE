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

#include <vulkan/vulkan.h>

namespace MAGE
{
	struct MemoryProps final
	{
		usize blockSize = 0;
		VkMemoryPropertyFlags memoryType;
	};

	class VMemory final : public VkObject
	{
		friend class VBuffer;
		friend class VDescBuffer;

		struct SubMemory
		{
			u64 sizeInBytes;
			b8 inUse;
		};

	public:
		VMemory(const MemoryProps& desc, VDevice* device);
		~VMemory() override final;

		u64 Allocate(usize allocSize);
		void Free(usize freeSize, usize offset);
		void FlushAll();
		void Destroy() override final;

		u64 GetRelaxedFreeSize() const;
		inline u64 GetTotalSize() const { return m_props.blockSize; }
		inline u64 GetFreeSize() const { return m_freeSize; }
		inline VkMemoryPropertyFlags GetType() const { return m_props.memoryType; }

	private:
		MemoryProps m_props;

		VkDeviceMemory m_memory;
		VkDeviceAddress m_address;

		u64 m_freeSize;

		vector<SubMemory> m_subMemories;

		u8* m_mappedData;
	};
}