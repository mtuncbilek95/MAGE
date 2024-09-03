#pragma once

#include "Engine/Core/Definitions.h"

namespace MAGE::Base
{
	template<typename T>
	class Vector
	{
	public:

		class Iterator
		{

		};

	public:

		Vector();
		Vector(u32 size);
		Vector(const Vector& other);
		Vector(Vector&& other) noexcept;
		~Vector();

		T* Data() const { return mData; }
		u32 Size() const { return mSize; }
		u32 Capacity() const { return mCapacity; }

		T& operator[](u32 index) { return mData[index]; }
		const T& operator[](u32 index) const { return mData[index]; }

		Vector& operator=(const Vector& other);
		Vector& operator=(Vector&& other) noexcept;

		void PushBack(const T& value);
		void PushFront(const T& value);
		void PopBack();

		void EmplaceBack(T&& value);

		void Clear();
		void Shrink();

		void Resize(u32 size);
		void Reserve(u32 capacity);

	private:
		T* mData;
		u32 mSize;
		u32 mCapacity;
	};
}