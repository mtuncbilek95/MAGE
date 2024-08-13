#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Platform/PlatformCriticalSection.h>

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	class Win32CriticalSection : public PlatformCriticalSection
	{
	public:
		Win32CriticalSection();
		~Win32CriticalSection() override = default;

		b8 TryEnter() override;
		void Enter() override;
		void Exit() override;

	private:
		CRITICAL_SECTION mCriticalSection;
	};
}
