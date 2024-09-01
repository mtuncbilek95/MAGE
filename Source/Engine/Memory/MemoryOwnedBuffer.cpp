#include "MemoryOwnedBuffer.h"

namespace MAGE
{
	MemoryOwnedBuffer::MemoryOwnedBuffer() : mBufferData(nullptr), mBufferSize(0)
	{
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(buf pData, u64 size) : mBufferData(nullptr), mBufferSize(0)
	{
		Allocate(pData, size);
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(const MemoryOwnedBuffer& pOther) : mBufferData(nullptr), mBufferSize(0)
	{
		Allocate(pOther.mBufferData, pOther.mBufferSize);
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(MemoryOwnedBuffer&& pOther) noexcept : mBufferData(pOther.mBufferData), mBufferSize(pOther.mBufferSize)
	{
		pOther.mBufferData = nullptr;
		pOther.mBufferSize = 0;
	}

	MemoryOwnedBuffer::~MemoryOwnedBuffer()
	{
		delete[] static_cast<i8*>(mBufferData);
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator+(MemoryOwnedBuffer& pOther)
	{
		MemoryOwnedBuffer newBuffer;
		if (pOther.mBufferData && pOther.mBufferSize > 0)
		{
			u64 newSize = mBufferSize + pOther.mBufferSize;
			void* newData = new i8[newSize];

			std::memcpy(newData, mBufferData, mBufferSize);
			std::memcpy(static_cast<i8*>(newData) + mBufferSize, pOther.mBufferData, pOther.mBufferSize);

			newBuffer.mBufferData = newData;
			newBuffer.mBufferSize = newSize;
		}

		return *this;
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator+=(MemoryOwnedBuffer& pOther)
	{
		if (pOther.mBufferData && pOther.mBufferSize > 0)
		{
			u64 newSize = mBufferSize + pOther.mBufferSize;
			void* newData = new i8[newSize];

			std::memcpy(newData, mBufferData, mBufferSize);
			std::memcpy(static_cast<i8*>(newData) + mBufferSize, pOther.mBufferData, pOther.mBufferSize);

			delete[] static_cast<i8*>(mBufferData);

			mBufferData = newData;
			mBufferSize = newSize;
		}
		return *this;
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator-(MemoryOwnedBuffer& pOther)
	{
		MemoryOwnedBuffer newBuffer;
		if (pOther.mBufferData && pOther.mBufferSize > 0)
		{
			u64 newSize = mBufferSize - pOther.mBufferSize;
			void* newData = new i8[newSize];

			std::memcpy(newData, mBufferData, newSize);

			newBuffer.mBufferData = newData;
			newBuffer.mBufferSize = newSize;
		}
		return *this;
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator-=(MemoryOwnedBuffer& pOther)
	{
		if (pOther.mBufferData && pOther.mBufferSize > 0)
		{
			u64 newSize = mBufferSize - pOther.mBufferSize;
			void* newData = new i8[newSize];

			std::memcpy(newData, mBufferData, newSize);

			delete[] static_cast<i8*>(mBufferData);

			mBufferData = newData;
			mBufferSize = newSize;
		}
		return *this;
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator=(MemoryOwnedBuffer& pOther)
	{
		if (this != &pOther)
		{
			void* newData = nullptr;
			if (pOther.mBufferData && pOther.mBufferSize > 0)
			{
				newData = new i8[pOther.mBufferSize];
				std::memcpy(newData, pOther.mBufferData, pOther.mBufferSize);
			}

			delete[] static_cast<i8*>(mBufferData);

			mBufferData = newData;
			mBufferSize = pOther.mBufferSize;
		}
		return *this;
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator=(MemoryOwnedBuffer&& pOther) noexcept
	{
		if (this != &pOther)
		{
			delete[] static_cast<i8*>(mBufferData);

			mBufferData = pOther.mBufferData;
			mBufferSize = pOther.mBufferSize;

			pOther.mBufferData = nullptr;
			pOther.mBufferSize = 0;
		}
		return *this;
	}

	void MemoryOwnedBuffer::Allocate(const buf pData, const u64 size)
	{
		if (pData && size > 0)
		{
			mBufferData = new i8[size];
			std::memcpy(mBufferData, pData, size);
			mBufferSize = size;
		}
	}
}