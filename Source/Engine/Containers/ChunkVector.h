#pragma once

#include "Engine/Core/Definitions.h"

namespace MAGE::Base
{
	/**
	* @class StackVector
	* @brief A vector that works as static chunk arrays.
	* 
	* @note Use this for incredibly large arrays.
	*/
	template<typename T>
	class ChunkVector
	{
		struct ChunkArray
		{
			ChunkArray* Next;
			ChunkArray* Prev;
			u64 mChunkSize;
			i64 mUsedIndex;
			bool* mFill;
			T mData[0];

			ChunkArray(u64 chunkSize) : mChunkSize(chunkSize), mUsedIndex(-1)
			{
				m_fill = reinterpret_cast<bool*>(m_data + size);
				memset(m_fill, 0, size * sizeof(bool));
			}

			~ChunkArray()
			{
				for (u64 i = 0; i < mChunkSize; ++i)
				{
					if (mFill[i])
						mData[i].~T();
				}

				mNext = nullptr;
				mPrev = nullptr;
			};
		};

	public:
		class Iterator
		{

		};

	public:
		ChunkVector()

	private:
		ChunkArray* mHead;
		ChunkArray* mTail;
		u64 mSize;
		u64 mCapacity;
	};
}