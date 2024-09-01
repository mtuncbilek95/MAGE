#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class PipelineBindPoint : u8
	{
		Graphics,
		Compute,
		RayTracing
	};
}