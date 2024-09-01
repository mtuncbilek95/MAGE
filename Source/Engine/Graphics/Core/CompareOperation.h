#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class CompareOperation : u8
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