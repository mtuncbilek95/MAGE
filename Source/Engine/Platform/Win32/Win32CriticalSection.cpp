#include "Win32CriticalSection.h"

namespace MAGE
{
	Win32CriticalSection::Win32CriticalSection()
	{
#if defined(MAGE_WINDOWS)
		InitializeCriticalSectionAndSpinCount(&mCriticalSection, u64_max);
#endif
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
