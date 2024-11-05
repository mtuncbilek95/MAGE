/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Definitions.h"
#include "Engine/Core/StdFix.h"

#include <string>
#include <string_view>
#include <algorithm>

namespace MAGE
{
	class StringView;

	class String final
	{
	public:
		String() = default;
		String(const char* str) : m_string(str) {}
		String(const char* str, usize size) : m_string(str, size) {}
		String(const std::string& str) : m_string(str) {}
		String(const std::string_view& str) : m_string(str) {}
		String(const StringView& str);
		String(const String& other) : m_string(other.m_string) {}
		String(String&& other) noexcept : m_string(std::move(other.m_string)) {}
		String(std::string&& other) noexcept : m_string(std::move(other)) {}
		~String() = default;

		template<typename T> String& operator=(const T& other)
		{
			m_string = other;
			return *this;
		}

		template<typename T> String& operator=(T&& other) noexcept
		{
			m_string = std::move(other);
			return *this;
		}

		String& operator=(const String& other)
		{
			m_string = other.m_string;
			return *this;
		}

		String& operator=(String&& other) noexcept
		{
			m_string = std::move(other.m_string);
			return *this;
		}

		template<typename T> String& operator+=(const T& other)
		{
			m_string += other;
			return *this;
		}

		String& operator+=(const String& other)
		{
			m_string += other.m_string;
			return *this;
		}

		template<typename T> String operator+(const T& other) const { return m_string + other; }
		String operator+(const String& other) const { return m_string + other.m_string; }

		template<typename T> auto operator<=>(const T& other) const { return m_string <=> other; }
		auto operator<=>(const String& other) const = default;
		explicit operator b8() const { return !m_string.empty(); }

		operator std::string() const { return m_string; }
		operator std::string_view() const { return m_string; }

		char& operator[](usize index) noexcept { return m_string[index]; }
		const char& operator[](usize index) const noexcept { return m_string[index]; }
		char& At(usize index) { return m_string.at(index); }
		const char& At(usize index) const { return m_string.at(index); }
		const char* CharString() const { return m_string.c_str(); }
		const char* Data() const { return m_string.data(); }
		usize Length() const { return m_string.length(); }
		usize Size() const { return m_string.size(); }
		b8 Empty() const { return m_string.empty(); }
		usize Capacity() const { return m_string.capacity(); }
		void Resize(usize length) { m_string.resize(length); }

		char& Front() { return m_string.front(); }
		char& Back() { return m_string.back(); }
		void PopBack() { m_string.pop_back(); }
		void Clear() { m_string.clear(); }
		void ShrinkToFit() { m_string.shrink_to_fit(); }
		String SubString(usize start, usize elementCount) const { return m_string.substr(start, elementCount); }
		String SubString(usize count) const { return m_string.substr(count); }
		b8 StartsWith(const std::string_view& str) const { return m_string.find(str) == 0; }
		b8 EndsWith(const std::string_view& str) const { return m_string.rfind(str) == m_string.size() - str.size(); }
		b8 Contains(const std::string_view& str) const { return m_string.find(str) != std::string::npos; }

		usize FindStartIndexOf(const std::string_view& str) const { return m_string.find(str); }
		usize FindEndIndexOf(const std::string_view& str) const { return m_string.rfind(str); }

		static constexpr auto NPos = std::string::npos;

		String& Trim() noexcept { return TrimBack().TrimFront(); }

		String Trim() const { return Trim(); }

		String& TrimFront() noexcept
		{
			usize start = m_string.find_first_not_of(" \t\n\r");
			if (start == std::string::npos)
			{
				m_string.clear();
			}
			else
			{
				m_string.erase(0, start);
			}
			return *this;
		}

		String& TrimBack() noexcept
		{
			usize end = m_string.find_last_not_of(" \t\n\r");
			if (end == std::string::npos)
			{
				m_string.clear();
			}
			else
			{
				m_string.resize(end + 1);
			}
			return *this;
		}

		Vector<String> Split(const std::string_view& delimiter) const
		{
			Vector<String> result;
			usize start = 0;
			usize end = m_string.find(delimiter);
			while (end != std::string::npos)
			{
				result.push_back(m_string.substr(start, end - start));
				start = end + delimiter.size();
				end = m_string.find(delimiter, start);
			}
			result.push_back(m_string.substr(start, end));
			return result;
		}

		Pair<String, String> SplitAtLastOccurrence(const std::string_view& delimiter) const
		{
			usize end = m_string.rfind(delimiter);
			if (end == std::string::npos)
			{
				return { m_string, "" };
			}
			return { m_string.substr(0, end), m_string.substr(end + delimiter.size()) };
		}

		Pair<String, String> SplitAtFirstOccurrence(const std::string_view& delimiter) const
		{
			usize end = m_string.find(delimiter);
			if (end == std::string::npos)
			{
				return { m_string, "" };
			}
			return { m_string.substr(0, end), m_string.substr(end + delimiter.size()) };
		}

		Vector<std::string_view> SplitAsStringViews(const std::string_view& delimiter) const
		{
			Vector<std::string_view> result;
			usize start = 0;
			usize end = m_string.find(delimiter);
			while (end != std::string::npos)
			{
				result.push_back(std::string_view(m_string.c_str() + start, end - start));
				start = end + delimiter.size();
				end = m_string.find(delimiter, start);
			}
			result.push_back(
				std::string_view(m_string.c_str() + start, m_string.size() - (start + end + delimiter.size())));
			return result;
		}

		Pair<std::string_view, std::string_view>
			SplitAtLastOccurenceAsStringViews(const std::string_view& delimiter) const
		{
			usize end = m_string.rfind(delimiter);
			if (end == std::string::npos)
			{
				return { m_string, "" };
			}
			return { std::string_view(m_string.c_str(), end),
					 std::string_view(m_string.c_str() + end + delimiter.size(),
									  m_string.size() - end - delimiter.size()) };
		}

		Pair<std::string_view, std::string_view>
			SplitAtFirstOccurenceAsStringViews(const std::string_view& delimiter) const
		{
			usize end = m_string.find(delimiter);
			if (end == std::string::npos)
			{
				return { m_string, "" };
			}
			return { std::string_view(m_string.c_str(), end),
					 std::string_view(m_string.c_str() + end + delimiter.size(),
									  m_string.size() - end - delimiter.size()) };
		}

		static inline i64 OctToInt(const std::string_view& string)
		{
			i64 result = 0;
			for (int i = 0; i < static_cast<int>(string.length()); i++)
			{
				char c = string[i];
				if (c == 0)
				{
					break;
				}
				if (c == ' ')
				{
					continue;
				}
				if (c < '0' || c > '7')
				{
					return -1;
				}
				result = result * 8 + c - '0';
			}
			return result;
		}

		i64 OctToInt() const { return OctToInt(m_string); }

		static inline i64 HexToInt(const std::string_view& string)
		{
			i64 result = 0;
			for (char c : string)
			{
				if (c == 0)
				{
					break;
				}
				if (c == ' ')
				{
					continue;
				}
				if (c >= '0' && c <= '9')
				{
					result = result * 16 + c - '0';
				}
				else if (c >= 'A' && c <= 'F')
				{
					result = result * 16 + c - 'A' + 10;
				}
				else if (c >= 'a' && c <= 'f')
				{
					result = result * 16 + c - 'a' + 10;
				}
				else
				{
					return -1;
				}
			}
			return result;
		}

		i64 HexToInt() { return HexToInt(m_string); }

		void ToUpper() noexcept
		{
			std::transform(m_string.begin(), m_string.end(), m_string.begin(),
				[](unsigned char c) { return std::toupper(c); });
		}

		void ToLower() noexcept
		{
			std::transform(m_string.begin(), m_string.end(), m_string.begin(),
				[](unsigned char c) { return std::tolower(c); });
		}

		void Capitalize() noexcept
		{
			if (!m_string.empty())
			{
				m_string[0] = std::toupper(m_string[0]);
			}
		}

		std::string::iterator begin() { return m_string.begin(); }
		std::string::iterator end() { return m_string.end(); }
		std::string::const_iterator cbegin() const { return m_string.cbegin(); }
		std::string::const_iterator cend() const { return m_string.cend(); }
		std::string::reverse_iterator rbegin() { return m_string.rbegin(); }
		std::string::reverse_iterator rend() { return m_string.rend(); }
		std::reverse_iterator<std::string::const_iterator> crbegin() const { return m_string.crbegin(); }
		std::reverse_iterator<std::string::const_iterator> crend() const { return m_string.crend(); }

	private:
		std::string m_string;
	};
	
	template<typename T> String operator+(const T& lhs, const String& rhs) noexcept { return String(lhs) + rhs; }
}
