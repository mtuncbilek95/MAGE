#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct MonitorDesc
	{
		String MonitorName;
		Math::Vec2u MonitorResolution;
		Math::Vec2i MonitorPosition;
		b8 IsPrimary;
	};
}
