#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class TextureAspectFlags : u8
	{
		TAF_Undefined = 0,
		TAF_ColorAspect = 1 << 0,
		TAF_DepthAspect = 1 << 1,
		TAF_StencilAspect = 1 << 2,
		TAF_MetaAspect = 1 << 3
	};
}
