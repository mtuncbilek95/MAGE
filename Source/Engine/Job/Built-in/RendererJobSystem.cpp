#include "RendererJobSystem.h"

namespace MAGE
{
	RendererJobSystem::RendererJobSystem(u32 threadCount, WorkerThreadPriority priority) : JobSystem(threadCount, priority)
	{
		mWorkers.resize(threadCount);
		for (u32 i = 0; i < threadCount; ++i)
		{
			mWorkers[i] = MakeShared<WorkerThread>(priority);
		}
	}
}