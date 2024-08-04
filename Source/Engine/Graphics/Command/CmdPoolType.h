#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class CmdPoolType : u8
	{
		Graphics = 1,
		Compute = 2,
		Transfer = 4
	};
}