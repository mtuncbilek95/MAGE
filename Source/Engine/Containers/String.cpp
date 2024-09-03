#include "String.h"

#include <iostream>

namespace MAGE::Base
{
	u64 GetStringLength(const i8* str)
	{
		u64 length = 0;
		while (*str++)
			++length;
		return length;
	}

	String::String() : mData(nullptr), mSize(0)
	{
		mData = static_cast<i8*>(malloc(1));
		if (mData)
			mData[0] = '\0';
	}

	String::String(const i8* str) : mSize(GetStringLength(str))
	{
		if (str == nullptr)
		{
			mSize = 0;
			mData = static_cast<i8*>(malloc(1));
			if (mData)
				mData[0] = '\0';
			return;
		}

		mSize = GetStringLength(str);
		mData = static_cast<i8*>(malloc(mSize + 1));
		memcpy_s(mData, mSize, str, mSize);
		if(mData)
			mData[mSize] = '\0';

	}

	String::String(const String& other) : mSize(other.mSize)
	{
		mData = static_cast<i8*>(malloc(mSize + 1));
		memcpy_s(mData, mSize, other.mData, mSize);
		if (mData)
			mData[mSize] = '\0';
	}

	String::String(String&& other) noexcept
	{
		mData = std::move(other.mData);
		mSize = std::move(other.mSize);

		other.mData = nullptr;
		other.mSize = 0;
	}

	String::~String()
	{
		if (mData)
			free(mData);
	}
}