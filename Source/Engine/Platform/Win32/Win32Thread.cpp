#include "Win32Thread.h"

namespace MAGE
{
	void Win32Thread::SleepCurrentThread(u64 ms)
	{
#if defined(MAGE_WINDOWS)
		Sleep((DWORD)ms);
#endif
	}

	u64 Win32Thread::GetCurrentThreadID()
	{
#if defined(MAGE_WINDOWS)
		return GetCurrentThreadId();
#endif
	}

	void Win32Thread::SetCurrentThreadPriority(u32 priority)
	{
#if defined(MAGE_WINDOWS)
		SetThreadPriority(GetCurrentThread(), priority);
#endif
	}

	Win32Thread::Win32Thread(u64 stackSize, ThreadJob* pJob) : PlatformThread(stackSize, pJob)
	{
#if defined(MAGE_WINDOWS)
		mThreadHandle = CreateThread(nullptr, stackSize, (LPTHREAD_START_ROUTINE)HandleThread, pJob, 0, nullptr);
#endif
	}

	void Win32Thread::HandleThread(void* pParam)
	{
		ThreadJob* pJob = static_cast<ThreadJob*>(pParam);
		//pJob->Execute();
	}
}
