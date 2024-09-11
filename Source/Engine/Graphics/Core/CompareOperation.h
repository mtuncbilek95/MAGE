#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum CompareOperation
	 * @brief Enum for compare operation
	 */
	enum class ENGINE_API CompareOperation : u8
	{
		CO_Never,
		CO_Less,
		CO_Equal,
		CO_LessEqual,
		CO_Greater,
		CO_NotEqual,
		CO_GreaterEqual,
		CO_Always
	};
}