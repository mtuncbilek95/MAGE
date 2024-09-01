#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Shader/ShaderStage.h"

namespace MAGE
{
	struct PushConstantRange
	{
		ShaderStage Stage;
		u32 Offset;
		u32 Size;
	};

	struct PushConstantDesc
	{
		Vector<PushConstantRange> PushConstantRanges;
	};
}
