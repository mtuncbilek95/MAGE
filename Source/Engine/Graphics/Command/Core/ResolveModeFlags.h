#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ResolveModeFlags : u8
	{
		RMF_None,
		RMF_SampleZero,
		RMF_Average,
		RMF_Min,
		RMF_Max
	};
}
