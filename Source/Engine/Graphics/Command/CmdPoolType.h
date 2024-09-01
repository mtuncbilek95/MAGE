#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class CmdPoolType : u8
	{
		CPT_Graphics = 1,
		CPT_Compute = 2,
		CPT_Transfer = 4
	};
}