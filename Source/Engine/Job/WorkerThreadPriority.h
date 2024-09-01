#pragma once

namespace MAGE
{
	enum class WorkerThreadPriority
	{
#if defined(MAGE_WINDOWS)
		WTP_Idle = -15,
		WTP_Lowest = -2,
		WTP_BelowNormal = -1,
		WTP_Normal = 0,
		WTP_AboveNormal = 1,
		WTP_Highest = 2,
		WTP_Realtime = 15
#endif
	};
}