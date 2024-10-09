#include "OwnedBuffer.h"

namespace MAGE
{
	OwnedBuffer::OwnedBuffer() : mBufferData(nullptr), mBufferSize(0)
	{
	}

	OwnedBuffer::OwnedBuffer(buf pData, usize size) : mBufferData(nullptr), mBufferSize(0)
	{
		Allocate(pData, size);
	}

	OwnedBuffer::OwnedBuffer(const OwnedBuffer& pOther) : mBufferData(nullptr), mBufferSize(0)
	{
		Allocate(pOther.mBufferData, pOther.mBufferSize);
	}

	OwnedBuffer::OwnedBuffer(OwnedBuffer&& pOther) noexcept : mBufferData(pOther.mBufferData), mBufferSize(pOther.mBufferSize)
	{
		pOther.mBufferData = nullptr;
		pOther.mBufferSize = 0;
	}

	OwnedBuffer::~OwnedBuffer()
	{
		delete[] static_cast<char*>(mBufferData);
	}

	OwnedBuffer& OwnedBuffer::operator=(OwnedBuffer& pOther)
	{
		if (this != &pOther)
		{
			void* newData = nullptr;
			if (pOther.mBufferData && pOther.mBufferSize > 0)
			{
				newData = new char[pOther.mBufferSize];
				std::memcpy(newData, pOther.mBufferData, pOther.mBufferSize);
			}

			delete[] static_cast<char*>(mBufferData);

			mBufferData = newData;
			mBufferSize = pOther.mBufferSize;
		}
		return *this;
	}

	OwnedBuffer& OwnedBuffer::operator=(OwnedBuffer&& pOther) noexcept
	{
		if (this != &pOther)
		{
			delete[] static_cast<char*>(mBufferData);

			mBufferData = pOther.mBufferData;
			mBufferSize = pOther.mBufferSize;

			pOther.mBufferData = nullptr;
			pOther.mBufferSize = 0;
		}
		return *this;
	}

	void OwnedBuffer::Allocate(const buf pData, const usize size)
	{
		if (pData && size > 0)
		{
			mBufferData = new char[size];
			std::memcpy(mBufferData, pData, size);
			mBufferSize = size;
		}
	}
} // namespace IOAlloc
