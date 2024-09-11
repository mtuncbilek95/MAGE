#pragma once

#include "Engine/Core/Core.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	struct Time final
	{
		u16 Day;
		u16 Month;
		u16 Year;

		u16 Hour;
		u16 Minute;
		u16 Second;

		u16 Millisecond;
	};

	class ENGINE_API PlatformTime 
	{
		static PlatformTime& Get();
	public:
		PlatformTime();
		~PlatformTime() = default;

		void Start();
		void Stop();

		f64 GetInNanoSec();
		f64 GetInMicroSec();
		f64 GetInMilliSec();
		f64 GetInSec();

		static const Time GetDateTime();

	private:
#if defined(MAGE_WINDOWS)
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mEndTime;
		LARGE_INTEGER mFrequency;
#endif
	};
}