#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum LogicOperation
	 * @brief Enum for logic operation
	 */
	enum class ENGINE_API LogicOperation : u8
	{
		LO_Clear,
		LO_Set,
		LO_Copy,
		LO_CopyInverted,
		LO_NoOp,
		LO_Invert,
		LO_And,
		LO_NotAnd,
		LO_Or,
		LO_Nor,
		LO_Xor,
		LO_Equal,
		LO_AndReverse,
		LO_AndInverted,
		LO_OrReverse,
		LO_OrInverted
	};
}