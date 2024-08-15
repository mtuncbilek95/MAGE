#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class WorkerThreadPriority
	{
#if defined(MAGE_WINDOWS)
		Idle = -15,
		Lowest = -2,
		BelowNormal = -1,
		Normal = 0,
		AboveNormal = 1,
		Highest = 2,
		Realtime = 15
#endif
	};
}