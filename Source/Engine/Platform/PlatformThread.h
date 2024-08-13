#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class JobPriority
	{
#if defined(MAGE_WINDOWS)
		Realtime = 15,
		High = 2,
		AboveNormal = 1,
		Normal = 0,
		BelowNormal = -1,
		Low = -2,
		Background = -15
#endif
	};

	class ThreadJob;
	class PlatformThread
	{
	public:
		template<typename T, typename... Args>
		static SharedPtr<PlatformThread> Create(u64 stackSize, Args... params)
		{
			return Handle(stackSize, new T(params...));
		}

		static SharedPtr<PlatformThread> Create(u64 stackSize, ThreadJob* pJob)
		{
			return Handle(stackSize, pJob);
		}

		static void Sleep(u64 ms);
		static u64 GetCurrentThreadID();
		static void SetCurrentThreadPriority(JobPriority priority);

	public:
		PlatformThread(u64 stackSize, ThreadJob* pJob);
		virtual ~PlatformThread() = default;

		FORCEINLINE ThreadJob* GetJob() const { return mJob; }
		FORCEINLINE u64 GetStackSize() const { return mStackSize; }

	private:
		static SharedPtr<PlatformThread> Handle(u64 stackSize, ThreadJob* pJob);

	private:
		ThreadJob* mJob = nullptr;
		u64 mStackSize;
	};
}
