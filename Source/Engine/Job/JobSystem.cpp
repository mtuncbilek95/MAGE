#include "JobSystem.h"

namespace MAGE
{
	JobSystem::JobSystem(u32 threadCount, WorkerThreadPriority priority) : mThreadCount(threadCount)
		, mPriority(priority)
	{
	}

	JobSystem::~JobSystem()
	{
		WaitForAllThreads();

		mWorkers.clear();
		mWorkers.shrink_to_fit();
	}

	void JobSystem::AddJob(SharedPtr<ThreadJob> job)
	{
		mWorkers[mWorkerIndex++ % mThreadCount]->AddJob(job);
	}

	void JobSystem::AddTask(VoidFunction task)
	{
		mWorkers[mWorkerIndex++ % mThreadCount]->AddJob(task);
	}
}