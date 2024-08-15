#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Platform/PlatformThread.h>

#include <Windows.h>

namespace MAGE
{
	class Win32Thread final : public PlatformThread
	{
	public:
		static void SleepCurrentThread(u64 ms);
		static u64 GetCurrentThreadID();
	public:
		Win32Thread(ThreadJob* pJob);
		~Win32Thread() = default;

		FORCEINLINE HANDLE GetWin32Handle() const { return mThreadHandle; }

	protected:
		virtual void SetPriorityImpl(int priority) override;

	private:
		static void HandleThread(void* pParam);
	
	private:
		HANDLE mThreadHandle;
	};
}
