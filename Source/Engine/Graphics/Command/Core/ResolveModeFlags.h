#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class ResolveModeFlags : u8
	{
		RMF_None,
		SampleZero,
		Average,
		Min,
		Max
	};
}
