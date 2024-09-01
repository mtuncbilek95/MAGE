#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Thread/ThreadJobState.h"
#include "Engine/Platform/PlatformThread.h"

namespace MAGE
{
	class PlatformCriticalSection;
	class ThreadJob
	{
		friend class PlatformThread;

	public:
		ThreadJob(VoidFunction job);
		~ThreadJob() = default;

		void Execute()
		{
			SetState(ThreadJobState::Running);
			mFunction();
			SetState(ThreadJobState::Finished);
		}

		ThreadJobState GetState() noexcept;
		void SetState(ThreadJobState state) noexcept;

	private:
		VoidFunction mFunction;
		ThreadJobState mState;
		SharedPtr<PlatformCriticalSection> mCriticalSection;
	};
}
