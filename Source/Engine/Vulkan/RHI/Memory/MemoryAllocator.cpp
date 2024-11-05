#include "MemoryAllocator.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	MemoryAllocator::MemoryAllocator(VDevice* device) : VkObject(device)
	{

	}

	MemoryAllocator::~MemoryAllocator()
	{
		Destroy();
	}

	void MemoryAllocator::AllocateMemory(usize allocSize)
	{

	}

	void MemoryAllocator::FreeMemory(VMemory* memory, usize memSize, usize offset)
	{
	}

	void MemoryAllocator::Destroy()
	{

	}
}
