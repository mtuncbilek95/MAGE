#include "MemoryBuffer.h"
#include "MemoryOwnedBuffer.h"

namespace MAGE
{
	MemoryBuffer::MemoryBuffer() : mBuffer(nullptr), mSize(0)
	{
	}

	MemoryBuffer::MemoryBuffer(void* pData, const u64 size) : mBuffer(pData), mSize(size)
	{
	}

	MemoryBuffer::MemoryBuffer(const MemoryBuffer& other) : mBuffer(other.mBuffer), mSize(other.mSize)
	{
	}

	MemoryBuffer::MemoryBuffer(const MemoryOwnedBuffer& pOwned) : mBuffer(pOwned.GetData()), mSize(pOwned.GetSize())
	{
	}
}
