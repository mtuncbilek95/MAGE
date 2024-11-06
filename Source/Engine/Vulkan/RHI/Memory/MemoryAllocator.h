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
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	class VMemory;

	struct AllocProps final
	{
		AllocProps(usize size, vk::MemoryPropertyFlags type) : allocSize(size), memType(type) {}

		usize allocSize;
		vk::MemoryPropertyFlags memType;
	};

	class MemoryAllocator final : public VkObject
	{
	public:
		MemoryAllocator(VDevice* device);
		~MemoryAllocator() override final;

		VMemory* GetAvailableMemory(const AllocProps& allocDesc);
		void FreeMemory(VMemory* memory, usize memSize, usize offset);

		void Destroy() override final;

	private:
		List<Owned<VMemory>> m_memoryList;
	};
}
