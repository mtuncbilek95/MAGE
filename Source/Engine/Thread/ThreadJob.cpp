#include "ThreadJob.h"

#include <Engine/Platform/PlatformCriticalSection.h>

namespace MAGE
{
	ThreadJob::ThreadJob() : mState(ThreadJobState::Idle), mOwnerThread(nullptr), mQuitRequested(false)
	{
		mCriticalSection = PlatformCriticalSection::Create();
	}

	ThreadJobState ThreadJob::GetState()
	{
		ThreadJobState state;
		mCriticalSection->Enter();
		state = mState;
		mCriticalSection->Exit();

		return state;
	}

	void ThreadJob::Run()
	{
		SetState(ThreadJobState::Running);
		Execute();
		SetState(ThreadJobState::Finished);
	}

	void ThreadJob::RequestJobQuit()
	{
		mCriticalSection->Enter();
		mQuitRequested = true;
		mCriticalSection->Exit();
	}

	void ThreadJob::SetState(ThreadJobState state)
	{
		mCriticalSection->Enter();
		mState = state;
		mCriticalSection->Exit();
	}
}
