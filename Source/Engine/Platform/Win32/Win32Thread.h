#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Platform/PlatformThread.h>

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	class Win32Thread final : public PlatformThread
	{
	public:
		static void SleepCurrentThread(u64 ms);
		static u64 GetCurrentThreadID();
		static void SetCurrentThreadPriority(u32 priority);
	public:
		Win32Thread(u64 stackSize, ThreadJob* pJob);
		~Win32Thread() = default;

		FORCEINLINE HANDLE GetWin32Handle() const { return mThreadHandle; }

	private:
		static void HandleThread(void* pParam);
	private:
		HANDLE mThreadHandle;
	};
}
