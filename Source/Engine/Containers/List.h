#pragma once

#include "Engine/Core/Definitions.h"

namespace MAGE::Base
{
	/**
	* @class List
	* @brief A list container that works as a dynamic array. Basically
	* std::list but with lesser layers.
	*/
	template<typename T>
	class List
	{
		struct Node
		{
			T* Data;
			Node* Next;
			Node* Prev;
		};

	public:
		List();
		List(const List& other);
		List(List&& other) noexcept;
		~List();

		T* Data() const { return mHead->Data; }
		u32 Size() const { return mSize; }

		T& operator[](u32 index) { return mHead->Data[index]; }
		const T& operator[](u32 index) const { return mHead->Data[index]; }

		List& operator=(const List& other);
		List& operator=(List&& other) noexcept;

		void Push(const T& value);
		void Pop();

		void Clear();

	private:
		Node* mHead;
		Node* mTail;
		u64 mSize;
	};
}