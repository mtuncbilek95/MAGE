#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class SamplerAddressMode : u8
	{
		SAM_Repeat,
		SAM_MirroredRepeat,
		SAM_ClampToEdge,
		SAM_ClampToBorder,
		SAM_MirrorClampToEdge
	};
}
