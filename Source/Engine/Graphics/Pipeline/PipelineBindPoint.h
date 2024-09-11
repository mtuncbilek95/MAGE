#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ENGINE_API PipelineBindPoint : u8
	{
		PBP_Graphics,
		PBP_Compute,
		PBP_RayTracing
	};
}