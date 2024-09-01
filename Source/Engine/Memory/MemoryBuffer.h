#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	class MemoryOwnedBuffer;
	class MemoryBuffer
	{
		typedef void* buf;
	public:
		MemoryBuffer();
		MemoryBuffer(buf pData, u64 size);
		MemoryBuffer(const MemoryBuffer& pOther);
		MemoryBuffer(const MemoryOwnedBuffer& pOther);
		~MemoryBuffer();

		MemoryBuffer(MemoryBuffer&& pOther) = delete;
		MemoryBuffer& operator=(MemoryBuffer&& pOther) = delete;

		MemoryBuffer& operator=(const MemoryBuffer& pOther);

		buf Data() const { return mBufferData; }
		u64 Size() const { return mBufferSize; }

		operator bool() const { return mBufferData != nullptr; }

	private:
		buf mBufferData;
		u64 mBufferSize;
	};
}