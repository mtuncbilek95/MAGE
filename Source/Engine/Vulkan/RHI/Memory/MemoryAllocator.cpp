#include "MemoryAllocator.h"

#include "../Device/VDevice.h"
#include "../Memory/VMemory.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	constexpr u64 blockSize = MiBToByte((u64)256);

	MemoryAllocator::MemoryAllocator(VDevice* device) : VkObject(device)
	{
	}

	MemoryAllocator::~MemoryAllocator()
	{
		Destroy();
	}

	VMemory* MemoryAllocator::GetAvailableMemory(const AllocProps& allocDesc)
	{
		// If requested size is bigger than blocksize ignore everystep and create it.
		if (allocDesc.allocSize > blockSize)
		{
			spdlog::debug("Requested size is bigger than regular size. Creating a specific block!");

			MemoryProps prop(allocDesc.allocSize, allocDesc.memType);
			m_memoryList.push_front(MakeOwned<VMemory>(prop, m_rootDevice));
			return &*m_memoryList.front();
		}

		// If its either equal or lesser than block size, check all list.
		for (auto& memory : m_memoryList)
		{
			if (memory->GetType() == allocDesc.memType && memory->GetRelaxedFreeSize() >= allocDesc.allocSize)
			{
				spdlog::debug("Found the block that is meeting the demands.");
				return &*memory;
			}
		}

		spdlog::debug("Creating a new default memory block.");
		// If it cant find proper memory, create new one with default size.
		MemoryProps prop(blockSize, allocDesc.memType);
		m_memoryList.push_front(MakeOwned<VMemory>(prop, m_rootDevice));
		return &*m_memoryList.front();
	}

	void MemoryAllocator::Destroy()
	{
		m_memoryList.clear();
	}
}
