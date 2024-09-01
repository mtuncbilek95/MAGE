#pragma once

#include "Engine/Core/Core.h"

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

	class PlatformTime 
	{
		static PlatformTime& Get();
	public:
		PlatformTime() = default;
		virtual ~PlatformTime() = default;

		void Start();
		void Stop();

		f64 GetInNanoSec();
		f64 GetInMicroSec();
		f64 GetInMilliSec();
		f64 GetInSec();

		static const Time GetDateTime();

	protected:
		virtual f64 GetInNanoSecImpl() = 0;
		virtual f64 GetInMicroSecImpl() = 0;
		virtual f64 GetInMilliSecImpl() = 0;
		virtual f64 GetInSecImpl() = 0;

		virtual void StartImpl() = 0;
		virtual void StopImpl() = 0;
	};
}