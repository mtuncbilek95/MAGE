#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	class MemoryBuffer;
	class MemoryOwnedBuffer
	{
		typedef void* buf;
	public:
		MemoryOwnedBuffer();
		MemoryOwnedBuffer(buf pData, u64 size);
		MemoryOwnedBuffer(const MemoryOwnedBuffer& pOther);
		MemoryOwnedBuffer(MemoryOwnedBuffer&& pOther) noexcept;
		~MemoryOwnedBuffer();


		MemoryOwnedBuffer& operator+(MemoryOwnedBuffer& pOther);
		MemoryOwnedBuffer& operator+=(MemoryOwnedBuffer& pOther);
		MemoryOwnedBuffer& operator-(MemoryOwnedBuffer& pOther);
		MemoryOwnedBuffer& operator-=(MemoryOwnedBuffer& pOther);
		MemoryOwnedBuffer& operator=(MemoryOwnedBuffer& pOther);
		MemoryOwnedBuffer& operator=(MemoryOwnedBuffer&& pOther) noexcept;

		buf Data() const { return mBufferData; }
		u64 Size() const { return mBufferSize; }

		operator bool() const { return mBufferData != nullptr; }

	protected:
		void Allocate(const buf pData, const u64 size);
	private:
		buf mBufferData;
		u64 mBufferSize;
	};
}