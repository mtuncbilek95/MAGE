#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class CompareOperation : u8
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};
}