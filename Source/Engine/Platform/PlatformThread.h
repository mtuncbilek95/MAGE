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
		PlatformThread(ThreadJob* pJob);
		virtual ~PlatformThread() = default;

		FORCEINLINE ThreadJob* GetJob() const { return mJob; }
		FORCEINLINE void SetPriority(int priority);

	protected:
		virtual void SetPriorityImpl(int priority) = 0;

	private:
		ThreadJob* mJob = nullptr;
	};
}
