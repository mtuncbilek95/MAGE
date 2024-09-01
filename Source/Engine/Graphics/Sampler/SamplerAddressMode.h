#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class SamplerAddressMode : u8
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirrorClampToEdge
	};
}
