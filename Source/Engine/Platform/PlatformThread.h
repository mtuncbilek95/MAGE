#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	class ThreadJob;
	class PlatformThread
	{
	public:
		static SharedPtr<PlatformThread> CreatePlatformThread(ThreadJob* pJob);
	public:
		static void SleepThread(u64 ms);
		static u64 GetCurrentThreadID();

	public:
		explicit PlatformThread(ThreadJob* pJob);
		virtual ~PlatformThread() = default;

		ThreadJob* GetJob() const { return mJob; }
		void SetPriority(int priority);

	protected:
		virtual void SetPriorityImpl(int priority) = 0;

	private:
		ThreadJob* mJob = nullptr;
	};
}
