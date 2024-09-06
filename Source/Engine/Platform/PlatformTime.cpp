#include "PlatformTime.h"

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	PlatformTime& PlatformTime::Get()
	{
		static PlatformTime instance;
		return instance;
	}

	PlatformTime::PlatformTime() : mStartTime(), mEndTime(), mFrequency()
	{
	}

	void PlatformTime::Start()
	{
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mStartTime);
	}

	void PlatformTime::Stop()
	{
		QueryPerformanceCounter(&mEndTime);
	}

	f64 PlatformTime::GetInNanoSec()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return (elapsed.QuadPart * 1000000000.0) / mFrequency.QuadPart;
	}

	f64 PlatformTime::GetInMicroSec()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return (elapsed.QuadPart * 1000000.0) / mFrequency.QuadPart;
	}

	f64 PlatformTime::GetInMilliSec()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return (elapsed.QuadPart * 1000.0) / mFrequency.QuadPart;
	}

	f64 PlatformTime::GetInSec()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return static_cast<double>(elapsed.QuadPart) / mFrequency.QuadPart;
	}

	const Time PlatformTime::GetDateTime()
	{
		SYSTEMTIME pTime;
		GetSystemTime(&pTime);

		Time time;
		time.Year = pTime.wYear;
		time.Month = pTime.wMonth;
		time.Day = pTime.wDay;
		time.Hour = pTime.wHour;
		time.Minute = pTime.wMinute;
		time.Second = pTime.wSecond;
		time.Millisecond = pTime.wMilliseconds;

		return time;
	}
#endif
}