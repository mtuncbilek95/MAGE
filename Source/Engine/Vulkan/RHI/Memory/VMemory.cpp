#include "VMemory.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VMemory::VMemory(const MemoryProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_freeSize(desc.blockSize)
	{
		vk::MemoryAllocateFlagsInfo allocFlagInfo = {};
		allocFlagInfo.flags = vk::MemoryAllocateFlagBits::eDeviceAddress;

		vk::MemoryAllocateInfo allocInfo = {};
		allocInfo.allocationSize = desc.blockSize;
		allocInfo.memoryTypeIndex = device->FindMemoryType(desc.memoryType);
		allocInfo.pNext = &allocFlagInfo;

		ErrorUtils::VkAssert(device->GetVkDevice().allocateMemory(&allocInfo, nullptr, &m_memory), "VMemory");

		m_subMemories.emplace_back(desc.blockSize, false);
	}

	VMemory::~VMemory()
	{
		Destroy();
	}

	u64 VMemory::Allocate(usize allocSize)
	{
		if (allocSize > m_freeSize)
		{
			spdlog::critical("Somehow MAGE:Delusion chose smaller sized memory!");
			exit(-9999);
		}

		u64 returnOffset = 0;
		// Check if 
		for (auto it = m_subMemories.begin(); it != m_subMemories.end(); ++it)
		{
			// Skip if its inUse
			if (it->inUse)
			{
				returnOffset += it->sizeInBytes;
				continue;
			}

			// If current sub lesser than requested, go to the next sub
			if (it->sizeInBytes < allocSize)
			{
				returnOffset += it->sizeInBytes;
				continue;
			}

			// This means the block is in the right size
			SubMemory newBlock = { allocSize, true };
			m_freeSize -= allocSize;
			m_subMemories.insert(it, newBlock);

			return returnOffset;
		}

		spdlog::critical("Somehow MAGE:Delusion couldn't find proper block size in the available free size!");
		return u64_max;
	}

	void VMemory::Free(usize freeSize, usize offset)
	{
		u64 moveOffset = 0;
		for (auto it = m_subMemories.begin(); it != m_subMemories.end(); ++it)
		{
			// if moved offset lesser than given offset, go to the next sub
			if (moveOffset < offset)
			{
				moveOffset += it->sizeInBytes;
				continue;
			}

			// if size doesnt match give warning for now, because it can be ask for delete in range.
			if (it->sizeInBytes < freeSize)
				spdlog::warn("You're trying to free something bigger than the subblock. This is not implemented yet. It will free just the block.");
			
			it->inUse = false;
			m_freeSize += it->sizeInBytes;
		}

		// TODO: I hope vulkan does the same thing I'm thinking below. :)
		/**
		 * |****|****|****|****|****|****|																		|****|**************|****|****|
		 * |****|****|****|****|****|****|																		|****|**************|****|****|
		 * |USED|FREE|FREE|FREE|USED|FREE|  <- This needs to be someting like this after freeing the memory. -> |USED|*****FREE*****|USED|FREE|
		 * |****|****|****|****|****|****|																		|****|**************|****|****|
		 * |****|****|****|****|****|****|																		|****|**************|****|****|
		 * 
		 * But still I don't know what to do if small free blocks make my free size equal to requested size. This can cause problems.
		 * 
		 * |****|**************|****|****|
		 * |****|**************|****|****|
		 * |USED|*****FREE*****|USED|FREE| If those 2 free blocks in sum are exactly equals to my requested size, this can cause literally critical issue
		 * |****|**************|****|****|
		 * |****|**************|****|****|
		 * 
		 */
	}

	void VMemory::FlushAll()
	{
	}

	void VMemory::Destroy()
	{
		if (m_memory != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().freeMemory(m_memory);
			m_memory = VK_NULL_HANDLE;
		}
	}

	u64 VMemory::GetRelaxedFreeSize() const
	{
		// Find the biggest free memory block
		u64 freeSize = 0;
		for (auto it = m_subMemories.begin(); it != m_subMemories.end(); ++it)
		{
			if (!it->inUse)
				freeSize = it->sizeInBytes > freeSize ? it->sizeInBytes : freeSize;
		}

		return freeSize;
	}
}
