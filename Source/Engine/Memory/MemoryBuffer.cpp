#include "MemoryBuffer.h"
#include "MemoryOwnedBuffer.h"

namespace MAGE
{
	MemoryBuffer::MemoryBuffer() : mBufferData(nullptr), mBufferSize(0)
	{
	}

	MemoryBuffer::MemoryBuffer(buf pData, u64 size) : mBufferData(pData), mBufferSize(size)
	{
	}

	MemoryBuffer::MemoryBuffer(const MemoryBuffer& pOther) : mBufferData(pOther.mBufferData), mBufferSize(pOther.mBufferSize)
	{
	}

	MemoryBuffer::MemoryBuffer(const MemoryOwnedBuffer& pOther) : mBufferData(pOther.Data()), mBufferSize(pOther.Size())
	{
	}

	MemoryBuffer::~MemoryBuffer()
	{
		mBufferData = nullptr;
		mBufferSize = 0;
	}

	MemoryBuffer& MemoryBuffer::operator=(const MemoryBuffer& pOther)
	{
		mBufferData = pOther.mBufferData;
		mBufferSize = pOther.mBufferSize;
		return *this;
	}
}