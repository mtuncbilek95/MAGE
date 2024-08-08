#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	/**
	 * @enum EnumType
	 * 
	 * @brief Since we need to reflect the enums we have to know the enum's type we see in 
	 * enum class Foo : uint8_t. For the foo example, the EnumType would be u8.
	 */
	enum class EnumType
	{
		u8,
		i8,
		u16,
		i16,
		u32,
		i32,
		u64,
		i64
	};
}
