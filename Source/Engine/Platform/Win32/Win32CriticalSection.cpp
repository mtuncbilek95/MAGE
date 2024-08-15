#include "Win32CriticalSection.h"

namespace MAGE
{
	Win32CriticalSection::Win32CriticalSection()
	{
		CORE_ASSERT(InitializeCriticalSectionAndSpinCount(&mCriticalSection, u64_max), "Win32CriticalSection", "Failed to initialize critical section");
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
