#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	struct MonitorDesc
	{
		String MonitorName;
		Vec2u MonitorResolution;
		Vec2i MonitorPosition;
		bool IsPrimary;
	};
}
