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
	struct VMemory final
	{
		VkDeviceMemory memory = VK_NULL_HANDLE;
		u64 usedSize = 0;
		u64 totalSize = 0;
	};

	class MemoryAllocator final : Singleton<MemoryAllocator>
	{
	public:
		MemoryAllocator() = default;
		~MemoryAllocator() override final;

		void SetAllocSize(u64 mbSize);
		u64 AlignMemory(u64 size, u64 offset);
		VMemory* Allocate(u64 size);
		void Free(VMemory* memory, u64 offset);

	private:
		List<VMemory> m_memoryList;
		u64 m_byteSize;
	};
}
