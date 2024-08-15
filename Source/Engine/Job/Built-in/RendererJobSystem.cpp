#include "RendererJobSystem.h"

namespace MAGE
{
	RendererJobSystem::RendererJobSystem(u32 threadCount, WorkerThreadPriority priority)
	{
		CORE_ASSERT(threadCount > 0, "RendererJobSystem", "JobSystem must have at least one thread");

		mWorkers.resize(threadCount);
		for (u32 i = 0; i < threadCount; ++i)
		{
			mWorkers[i] = MakeOwned<WorkerThread>(priority);
		}
	}
}