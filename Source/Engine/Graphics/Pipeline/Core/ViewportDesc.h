#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct ViewportDesc
	{
		Math::Vec2u ViewportSize;
		Math::Vec2u OffsetSize;
		Math::Vec2f DepthRange;
	};
}