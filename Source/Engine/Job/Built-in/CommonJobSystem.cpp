#include "CommonJobSystem.h"

#include <Engine/Platform/PlatformHardware.h>
#include <Engine/Thread/ThreadJob.h>
#include <Engine/Job/WorkerThread.h>

namespace MAGE
{
	CommonJobSystem::CommonJobSystem()
	{
	}

	CommonJobSystem::~CommonJobSystem()
	{
	}

	bool CommonJobSystem::Initialize(u16 threadCount)
	{
		if (threadCount == 0)
		{
			CORE_LOG(M_WARNING, "Thread count initialized as 0, defaulting to 2 threads.");
			mWorkerThreads.push_back(MakeShared<WorkerThread>());
			mWorkerThreads.push_back(MakeShared<WorkerThread>());
			return true;
		}

		if (PlatformHardware::GetCPUThreadCount() < threadCount)
		{
			CORE_LOG(M_WARNING, "Thread count initialized as %d, but the system only has %d threads. Defaulting to 2 threads", threadCount, PlatformHardware::GetCPUThreadCount());
			mWorkerThreads.push_back(MakeShared<WorkerThread>());
			mWorkerThreads.push_back(MakeShared<WorkerThread>());
			return true;
		}

		for (u16 i = 0; i < threadCount; i++)
		{
			mWorkerThreads.push_back(MakeShared<WorkerThread>());
		}

		return false;
	}

	void CommonJobSystem::AddJob(ThreadJob* job)
	{
		mWorkerThreads[mDistributionIndex]->AddJob(job);
		mDistributionIndex = (mDistributionIndex + 1) % mWorkerThreads.size();
	}
}
