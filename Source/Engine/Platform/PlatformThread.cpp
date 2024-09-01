#include "PlatformThread.h"

#include "Engine/Thread/ThreadJob.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32Thread.h"
typedef MAGE::Win32Thread Dependency;
#endif

namespace MAGE
{
	SharedPtr<PlatformThread> PlatformThread::CreatePlatformThread(ThreadJob* pJob)
	{
		return MakeShared<Dependency>(pJob);
	}

	void PlatformThread::SleepThread(u64 ms)
	{
		Dependency::SleepCurrentThread(ms);
	}

	u64 PlatformThread::GetCurrentThreadID()
	{
		return Dependency::GetCurrentThreadID();
	}

	PlatformThread::PlatformThread(ThreadJob* pJob) : mJob(pJob)
	{
	}

	void PlatformThread::SetPriority(const int priority)
	{
		SetPriorityImpl(priority);
	}
}
