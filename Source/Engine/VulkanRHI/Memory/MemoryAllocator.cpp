#include "MemoryAllocator.h"

#include "Engine/Helpers/ByteConverter.h"

namespace MAGE
{
	MemoryAllocator::~MemoryAllocator()
	{
	}

	void MemoryAllocator::SetAllocSize(u64 mbSize)
	{
		m_byteSize = MbToByte(mbSize);
	}

	u64 MemoryAllocator::AlignMemory(u64 size, u64 offset)
	{
		return (size + offset - 1) & ~(offset - 1);
	}

	VMemory* MemoryAllocator::Allocate(u64 size)
	{
		return nullptr;
	}

	void MemoryAllocator::Free(VMemory* memory, u64 offset)
	{
		for (auto& mem : m_memoryList)
		{
			if (&mem == memory)
			{

			}
		}
	}
}
