#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ENGINE_API ShaderStage : u8
	{
		SS_Vertex = 1,
		SS_TessellationControl = 1 << 1,
		SS_TessellationEvaluation = 1 << 2,
		SS_Geometry = 1 << 3,
		SS_Fragment = 1 << 4,
		SS_Compute = 1 << 5
	};
}
