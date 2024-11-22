#include "VAllocator.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"
#include "../Memory/VMemory.h"

namespace MAGE
{
	constexpr u64 blockSize =  256 * 1024 * 1024;

	VAllocator::VAllocator(VDevice* device) : VkObject(device)
	{
	}

	VAllocator::~VAllocator()
	{
		Destroy();
	}

	VMemory* VAllocator::GetAvailableMemory(const AllocProps& allocDesc)
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

	void VAllocator::FreeMemory(VMemory* memory, usize memSize, usize offset)
	{
	}

	void VAllocator::Destroy()
	{
		m_memoryList.clear();
	}
}