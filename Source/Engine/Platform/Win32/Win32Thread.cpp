#include "Win32Thread.h"

#include "Engine/Thread/ThreadJob.h"

namespace MAGE
{
	void Win32Thread::SleepCurrentThread(u64 ms)
	{
		Sleep(ms);
	}

	u64 Win32Thread::GetCurrentThreadID()
	{
		return GetCurrentThreadId();
	}

	Win32Thread::Win32Thread(ThreadJob* pJob) : PlatformThread(pJob)
	{
		mThreadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HandleThread, pJob, 0, nullptr);
	}

	void Win32Thread::SetPriorityImpl(int priority)
	{
		SetThreadPriority(mThreadHandle, priority);
	}

	void Win32Thread::HandleThread(void* pParam)
	{
		if (pParam)
		{
			ThreadJob* pJob = static_cast<ThreadJob*>(pParam);
			pJob->Execute();
		}
	}
}
