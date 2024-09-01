#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Platform/PlatformTime.h"

#include <Windows.h>

namespace MAGE
{
	class Win32Time final : public PlatformTime
	{
	public:
		Win32Time();
		virtual ~Win32Time() = default;

		virtual void StartImpl() override;
		virtual void StopImpl() override;

		virtual f64 GetInNanoSecImpl() override;
		virtual f64 GetInMicroSecImpl() override;
		virtual f64 GetInMilliSecImpl() override;
		virtual f64 GetInSecImpl() override;

		static const Time GetDateTimeOS();

	private:
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mEndTime;
		LARGE_INTEGER mFrequency;
	};
}