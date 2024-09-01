#include "Win32CriticalSection.h"

#include <spdlog/spdlog.h>

namespace MAGE
{
	Win32CriticalSection::Win32CriticalSection()
	{
		if(!InitializeCriticalSectionAndSpinCount(&mCriticalSection, u32_max))
			spdlog::error("Failed to initialize critical section");
	}

	b8 Win32CriticalSection::TryEnter()
	{
		return TryEnterCriticalSection(&mCriticalSection);
	}

	void Win32CriticalSection::Enter()
	{
		EnterCriticalSection(&mCriticalSection);
	}

	void Win32CriticalSection::Exit()
	{
		LeaveCriticalSection(&mCriticalSection);
	}
}
