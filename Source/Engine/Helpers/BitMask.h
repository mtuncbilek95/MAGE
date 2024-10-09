/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T operator+(T lhs, T rhs)
	{
		using Type = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<Type>(lhs) + static_cast<Type>(rhs));
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T operator-(T lhs, T rhs)
	{
		using Type = std::underlying_type_t<T>::type;
		return static_cast<T>(static_cast<Type>(lhs) - static_cast<Type>(rhs));
	}

	/**
	 * /class BitMask
	 * @brief A class that handles bit masking operations for given enum class.
	 */
	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	class BitMask
	{
		using Type = std::underlying_type_t<T>;

	public:
		BitMask() : m_mask(0) {}
		explicit BitMask(T flag) : m_mask(static_cast<Type>(flag)) {}
		BitMask(const BitMask& other) : m_mask(other.m_mask) {}
		BitMask(BitMask&& other) noexcept : m_mask(std::move(other.m_mask)) {}
		~BitMask() = default;

		BitMask& operator=(const BitMask& other)
		{
			if (this != &other)
				m_mask = other.m_mask;

			return *this;
		}

		BitMask& operator=(BitMask&& other) noexcept
		{
			if (this != &other)
				m_mask = std::move(other.m_mask);

			return *this;
		}

		BitMask& operator=(T flag)
		{
			m_mask = static_cast<Type>(flag);
			return *this;
		}

		BitMask& operator|=(T flag)
		{
			m_mask |= static_cast<Type>(flag);
			return *this;
		}

		BitMask& operator&=(T flag)
		{
			m_mask &= static_cast<Type>(flag);
			return *this;
		}

		BitMask& operator^=(T flag)
		{
			m_mask ^= static_cast<Type>(flag);
			return *this;
		}

		BitMask operator~() const
		{
			BitMask temp;
			temp.m_mask = ~m_mask;
			return temp;
		}

		bool IsSet(T flag) const
		{
			return (m_mask & static_cast<Type>(flag)) == static_cast<Type>(flag);
		}

		void Set(T flag)
		{
			m_mask |= static_cast<Type>(flag);
		}

		bool HasAny(T flag) const
		{
			return (m_mask & static_cast<Type>(flag)) != 0;
		}

		bool HasAll(T flag) const
		{
			return (m_mask & static_cast<Type>(flag)) == static_cast<Type>(flag);
		}

		void Clear(T flag)
		{
			m_mask &= ~static_cast<Type>(flag);
		}

		void Toggle(T flag)
		{
			m_mask ^= static_cast<Type>(flag);
		}

		void Clear()
		{
			m_mask = 0;
		}

		operator Type() const { return m_mask; }
		operator bool() const { return m_mask != 0; }

	private:
		Type m_mask;
	};
}
