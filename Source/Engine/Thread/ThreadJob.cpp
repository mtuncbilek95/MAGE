#include "ThreadJob.h"

#include "Engine/Platform/PlatformCriticalSection.h"

namespace MAGE
{
	ThreadJob::ThreadJob(VoidFunction job) : mFunction(job), mState(ThreadJobState::TJS_Idle)
	{
		mCriticalSection = PlatformCriticalSection::CreateCS();
	}

	ThreadJobState ThreadJob::GetState() noexcept
	{
		mCriticalSection->Enter();
		ThreadJobState state = mState;
		mCriticalSection->Exit();
		return state;
	}

	void ThreadJob::SetState(ThreadJobState state) noexcept
	{
		mCriticalSection->Enter();
		mState = state;
		mCriticalSection->Exit();
	}
}
