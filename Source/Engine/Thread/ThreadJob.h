#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Thread/ThreadJobState.h>
#include <Engine/Platform/PlatformThread.h>

namespace MAGE
{
	typedef std::function<void()> JobFunc;

	class PlatformCriticalSection;
	class ThreadJob
	{
		friend class PlatformThread;

	public:
		ThreadJob();
		~ThreadJob() = default;

		ThreadJobState GetState();
		void Run();
		void RequestJobQuit();

	protected:
		virtual void Execute() = 0;

	private:
		void SetOwner(PlatformThread* pOwnerThread) { mOwnerThread = pOwnerThread; }
		void SetState(ThreadJobState state);

		PlatformThread* mOwnerThread;
		SharedPtr<PlatformCriticalSection> mCriticalSection;
		ThreadJobState mState = ThreadJobState::Idle;

		JobFunc mJobFunc;
		b8 mQuitRequested = false;
	};
}
