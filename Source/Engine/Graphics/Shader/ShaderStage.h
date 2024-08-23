#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class ShaderStage : u8
	{
		Vertex = 1,
		TessellationControl = 1 << 1,
		TessellationEvaluation = 1 << 2,
		Geometry = 1 << 3,
		Fragment = 1 << 4,
		Compute = 1 << 5
	};

	GENERATE_ENUM_FLAG(MAGE::ShaderStage, u8);
}
