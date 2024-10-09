#include "RawBuffer.h"
#include "OwnedBuffer.h"

namespace MAGE
{
	RawBuffer::RawBuffer() : mBufferData(nullptr), mBufferSize(0)
	{
	}

	RawBuffer::RawBuffer(buf pData, usize size) : mBufferData(pData), mBufferSize(size)
	{
	}

	RawBuffer::RawBuffer(const RawBuffer& pOther) : mBufferData(pOther.mBufferData), mBufferSize(pOther.mBufferSize)
	{
	}

	RawBuffer::RawBuffer(const OwnedBuffer& pOther) : mBufferData(pOther.Data()), mBufferSize(pOther.Size())
	{
	}

	RawBuffer::~RawBuffer()
	{
		mBufferData = nullptr;
		mBufferSize = 0;
	}

	RawBuffer& RawBuffer::operator=(const RawBuffer& pOther)
	{
		mBufferData = pOther.mBufferData;
		mBufferSize = pOther.mBufferSize;
		return *this;
	}
}
