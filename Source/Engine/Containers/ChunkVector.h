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
	template<typename T, u64 defChunkSize = 32, i32 coFactor = 2>
	class ChunkVector
	{
		/**
		 * @struct ChunkArray
		 * @brief Static array's for per chunk.
		 */
		struct ChunkArray
		{
			ChunkArray* Next = nullptr;
			ChunkArray* Prev = nullptr;
			u64 mChunkSize;
			i64 mUsedIndex;
			bool* mFill;
			T mData[0];

			ChunkArray(u64 chunkSize) : mChunkSize(chunkSize), mUsedIndex(-1)
			{
				mFill = reinterpret_cast<bool*>(mData + chunkSize);
				memset(mFill, 0, chunkSize * sizeof(bool));
			}

			~ChunkArray()
			{
				for (u64 i = 0; i < mChunkSize; ++i)
				{
					if (mFill[i])
						mData[i].~T();
				}

				Next = nullptr;
				Prev = nullptr;
			};
		};

	public:
		/**
		* @class Iterator
		* @brief An iterator for the ChunkVector. std's iterator is not used because
		* it's not compatible with this vector.
		*/
		class Iterator
		{
		public:
			Iterator(ChunkArray* ptr, u64 index = 0) : mPtr(ptr), mIndex(index)
			{}

			T& operator*() { return mPtr->mData[mIndex]; }
			T* operator->() { return &mPtr->mData[mIndex]; }

			Iterator operator++()
			{
				++mIndex;
				MovetoNextValidChunk();
				return *this;
			}

			Iterator operator++(i32)
			{
				Iterator temp = *this;
				++(*this);
				return temp;
			}

			bool operator==(const Iterator& other) const { return mPtr == other.mPtr && mIndex == other.mIndex; }
			bool operator!=(const Iterator& other) const { return !(*this == other); }

		protected:
			void MovetoNextValidChunk()
			{
				while (mPtr && (mIndex > mPtr->mChunkSize || !mPtr->mFill[mIndex]))
				{
					if (mIndex >= mPtr->mChunkSize)
					{
						mPtr = mPtr->Next;
						mIndex = 0;
					}
					else ++mIndex;
				}

				if (mPtr && mIndex >= mPtr->mChunkSize)
				{
					mPtr = mPtr->Next;
					mIndex = 0;
					MovetoNextValidChunk();
				}
			}

		private:
			ChunkArray* mPtr;
			u64 mIndex;
		};

	public:
		ChunkVector() : mFirstChunk(nullptr), mLastChunk(nullptr), mSize(0), mCapacity(defChunkSize)
		{
			mFirstChunk = SpawnChunk(defChunkSize);
			mLastChunk = mFirstChunk;
		}

		ChunkVector(const ChunkVector<T>& copy) : mFirstChunk(nullptr), mLastChunk(nullptr), mSize(copy.mSize), mCapacity(copy.mCapacity)
		{
			ChunkArray* currentChunk = copy.mFirstChunk;
			while (currentChunk)
			{
				for (u64 i = 0; i < currentChunk->mSize; i++)
					if (currentChunk->mFill[i]) { PushBack(currentChunk->mData[i]); }

				currentChunk = currentChunk->Next;
			}
		}

		ChunkVector(ChunkVector<T>&& move) noexcept : mFirstChunk(move.mFirstChunk), mLastChunk(move.mLastChunk), mSize(move.mSize), mCapacity(move.mCapacity)
		{
			move.mFirstChunk = nullptr;
			move.mLastChunk = nullptr;
			move.mSize = 0;
			move.mCapacity = 0;
		}

		~ChunkVector()
		{
			ChunkArray* currentChunk = mFirstChunk;
			while (currentChunk)
			{
				ChunkArray* next = currentChunk->Next;
				currentChunk->~ChunkArray();
				::operator delete(currentChunk);
				currentChunk = next;
			}
		}
		/**
		 * @function PushBack
		 * @brief Pushes the value to the back of the Vector.
		 */
		void PushBack(const T& value)
		{
			if (mLastChunk->mUsedIndex + 1 == mLastChunk->mChunkSize)
				ChunkArray* chunk = SpawnChunk(u64(mLastChunk->mChunkSize * coFactor));

			new (&mLastChunk->mData[++mLastChunk->mUsedIndex]) T(value);
			mLastChunk->mFill[mLastChunk->mUsedIndex] = true;
			mSize++;

		}
		/**
		 * @function EmplaceBack
		 * @brief Emplaces the value to the back of the Vector.
		 */
		template<typename... Args>
		void EmplaceBack(Args&&... args)
		{
			if (mLastChunk->mUsedIndex + 1 == mLastChunk->mChunkSize)
				ChunkArray* chunk = SpawnChunk(u64(mLastChunk->mChunkSize * coFactor));

			mLastChunk->mData[++mLastChunk->mUsedIndex] = T(std::forward<Args>(args)...);
			mLastChunk->mFill[mLastChunk->mUsedIndex] = true;
			mSize++;

		}
		/**
		 * @function Add
		 * @brief if you remove an element from the vector, it will leave a blank
		 * space. This function will emplace the first blank space with the value. If
		 * there is no empty space, it will act as EmplaceBack.
		 */
		template<typename... Args>
		void Emplace(Args&&... args) {}
		/**
		 * @function PopBack
		 * @brief Pops the value from the back of the Vector. It
		 * will leave a blank space.
		 */
		void PopBack() {}
		/*
		 * @function PopBackChunk
		 * @brief Pops the last chunk from the Vector.
		 */
		void PopBackChunk() {}
		/**
		 * @function Insert
		 * @brief Inserts the value to the specified index of the Vector. It will
		 * blow up the index if it's already filled with a value. Also it will resize
		 * the vector if the index is out of bounds.
		 */
		void Insert(u64 index, const T& value) {}

		/**
		 * @function Add
		 * @brief if you remove an element from the vector, it will leave a blank
		 * space. This function will fill the first blank space with the value. If
		 * there is no empty space, it will act as PushBack.
		 */
		void Add(const T& value)
		{
			ChunkArray* currentChunk = mFirstChunk;
			while (currentChunk)
			{
				for (u64 i = 0; i < currentChunk->mChunkSize; i++)
				{
					if (!currentChunk->mFill[i])
					{
						currentChunk->mData[i] = value;
						currentChunk->mFill[i] = true;
						mSize++;

						if (i > currentChunk->mUsedIndex) currentChunk->mUsedIndex = i;
						return;
					}
				}

				currentChunk = currentChunk->Next;
			}

			ChunkArray* chunk = SpawnChunk(u64(mLastChunk->mChunkSize * coFactor));

			new (&mLastChunk->m_data[++mLastChunk->mUsedIndex]) T(value);
			mLastChunk->mFill[mLastChunk->mUsedIndex] = true;
			mSize++;
		}

		T& operator[](u64 index) { return At(index); }
		T& At(u64 index) { return GetChunk(index)->mData[index]; }

		void Remove(u64 index) {}

		void Clear() {}

		Iterator begin() { return Iterator(mFirstChunk, 0); }
		Iterator end() { return Iterator(mLastChunk, mLastChunk->mUsedIndex + 1); }

		/**
		 * @function Size
		 * @brief Returns the total used size of the Vector
		 */
		u64 Size() const { return mSize; }
		/**
		 * @function Capacity
		 * @brief Returns the total capacity of the Vector
		 */
		u64 Capacity() const { return mCapacity; }

	protected:
		ChunkArray* SpawnChunk(u64 reqSize)
		{
			ChunkArray* chunk = static_cast<ChunkArray*>(::operator new(sizeof(ChunkArray) + reqSize * sizeof(T) + reqSize * sizeof(bool)));
			new (chunk)ChunkArray(reqSize);

			if (mLastChunk)
			{
				chunk->Prev = mLastChunk;
				mLastChunk->Next = chunk;
				mLastChunk = chunk;
				mCapacity += mLastChunk->mChunkSize;
			}

			return chunk;
		}

		ChunkArray* GetChunk(u64 localIndex)
		{
			ChunkArray* chunk = mFirstChunk;
			while (chunk)
			{
				if (localIndex > chunk->mChunkSize - 1)
				{
					localIndex -= chunk->mChunkSize;
					chunk = chunk->Next;
				}
				else break;
			}

			return chunk;
		}

	private:
		ChunkArray* mFirstChunk;
		ChunkArray* mLastChunk;
		u64 mSize;
		u64 mCapacity;
	};
}