#include "PlatformThread.h"

#include <Engine/Thread/ThreadJob.h>

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Thread.h"
typedef MAGE::Win32Thread PlatformDependency;
#endif

namespace MAGE
{
	void PlatformThread::Sleep(u64 ms)
	{
		PlatformDependency::SleepCurrentThread(ms);
	}

	u64 PlatformThread::GetCurrentThreadID()
	{
		return PlatformDependency::GetCurrentThreadID();
	}

	void PlatformThread::SetCurrentThreadPriority(JobPriority priority)
	{
		PlatformDependency::SetCurrentThreadPriority((u32)priority);
	}

	PlatformThread::PlatformThread(u64 stackSize, ThreadJob* pJob) : mStackSize(stackSize)
		, mJob(pJob)
	{
		mJob->SetOwner(this);
	}

	SharedPtr<PlatformThread> PlatformThread::Handle(u64 stackSize, ThreadJob* pJob)
	{
		return SharedPtr<PlatformThread>(new PlatformDependency(stackSize, pJob));
	}
}
