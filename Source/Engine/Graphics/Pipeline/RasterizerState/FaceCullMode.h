#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class FaceCullMode : u8
	{
		None = 1,
		Front = 2,
		Back = 4,
		All = Front | Back
	};
}