#include "PlatformThread.h"

#include <Engine/Thread/ThreadJob.h>

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Thread.h"
typedef MAGE::Win32Thread PlatformDependency;
#endif

namespace MAGE
{
	SharedPtr<PlatformThread> PlatformThread::CreatePlatformThread(ThreadJob* pJob)
	{
#if defined(MAGE_WINDOWS)
		return MakeShared<Win32Thread>(pJob);
#elif defined(MAGE_LINUX)
		return nullptr; // TODO: Implement Linux thread
#endif
	}

	void PlatformThread::SleepThread(u64 ms)
	{
#if defined(MAGE_WINDOWS)
		PlatformDependency::SleepCurrentThread(ms);
#endif
	}

	u64 PlatformThread::GetCurrentThreadID()
	{
#if defined(MAGE_WINDOWS)
		return PlatformDependency::GetCurrentThreadID();
#elif defined(MAGE_LINUX)
		return 0; // TODO: Implement Linux thread
#endif
	}

	PlatformThread::PlatformThread(ThreadJob* pJob) : mJob(pJob)
	{
	}

	void PlatformThread::SetPriority(const int priority)
	{
		SetPriorityImpl(priority);
	}
}
