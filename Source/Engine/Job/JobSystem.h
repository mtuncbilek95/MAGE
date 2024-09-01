#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Job/WorkerThread.h"

namespace MAGE
{
	class ThreadJob;

	class JobSystem
	{
	public:
		JobSystem(u32 threadCount, WorkerThreadPriority priority);
		virtual ~JobSystem();

		void AddJob(SharedPtr<ThreadJob> job);
		void AddTask(VoidFunction task);

		void WaitForAllThreads()
		{
			for (auto& worker : mWorkers)
				worker->RequestQuit();
		}

	protected:
		Vector<OwnedPtr<WorkerThread>> mWorkers;
		u32 mWorkerIndex = 0;

		u32 mThreadCount;
		WorkerThreadPriority mPriority;
	};
}
