#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Platform/PlatformCriticalSection.h"

#include <Windows.h>

namespace MAGE
{
	class ENGINE_API Win32CriticalSection : public PlatformCriticalSection
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
