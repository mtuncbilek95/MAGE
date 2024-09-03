#pragma once

#include "Engine/Core/Definitions.h"

namespace MAGE::Base
{
	/**
	* @class String
	* @brief Custom character string class like 
	* std::string with lesser layers.
	* 
	* @throw This class can reproduce the std::string to
	* create a better support between the 3rd party libraries.
	*/
	class String
	{
	public:
		String();
		String(const i8* str);
		String(const String& other);
		String(String&& other) noexcept;
		~String();

		i8* Data() const { return mData; }
		u32 Size() const { return mSize; }

		const i8* Str() const { return mData; }

		String& operator=(const String& other);
		String& operator=(String&& other) noexcept;

		String operator+(const String& other) const;
		String& operator+=(const String& other);

		String operator+(const i8* other) const;
		String& operator+=(const i8* other);

		operator bool() const { return mData != nullptr; }

		bool operator==(const String& other) const;
		bool operator!=(const String& other) const;

	private:
		i8* mData;
		u32 mSize;
	};
}