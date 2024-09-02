#pragma once

#include <type_traits>
#include <concepts>

/*
 * @class Add Bitwise
 * @brief Adds bitwise operators to an enum class
 */
template<typename EnumType>
inline constexpr EnumType operator |(EnumType a, EnumType b)
{
	using Underlying = std::underlying_type_t<EnumType>;
	return static_cast<EnumType>(static_cast<Underlying>(a) | static_cast<Underlying>(b));
}

/*
 * @class Bool Bitwise
 * @brief Checks if an enum variable contains a specific flag
 */
template<typename EnumType>
inline constexpr bool operator &(EnumType a, EnumType b)
{
	using Underlying = std::underlying_type_t<EnumType>;
	return (static_cast<Underlying>(a) & static_cast<Underlying>(b)) != 0;
}

namespace MAGE
{
	namespace Helpers
	{
		template<typename T>
		concept Arithmetic = std::is_arithmetic_v<T>;

		template<Arithmetic T>
		inline constexpr T MbToByte(T mb) { return mb * 1000.0 * 1000.0; }
		template<Arithmetic T>
		inline constexpr T KbToByte(T kb) { return kb * 1000.0; }
		template<Arithmetic T>
		inline constexpr T ByteToMb(T bt) { return bt / 1000.0 / 1000.0; }
		template<Arithmetic T>
		inline constexpr T ByteToKb(T bt) { return bt / 1000.0; }
	}
}
