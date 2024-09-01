#include "Win32Time.h"

namespace MAGE
{
	Win32Time::Win32Time() : mStartTime(), mEndTime(), mFrequency()
	{
	}

	void Win32Time::StartImpl()
	{
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mStartTime);
	}

	void Win32Time::StopImpl()
	{
		QueryPerformanceCounter(&mEndTime);
	}

	f64 Win32Time::GetInNanoSecImpl()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return (elapsed.QuadPart * 1000000000.0) / mFrequency.QuadPart;
	}

	f64 Win32Time::GetInMicroSecImpl()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return (elapsed.QuadPart * 1000000.0) / mFrequency.QuadPart;
	}

	f64 Win32Time::GetInMilliSecImpl()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return (elapsed.QuadPart * 1000.0) / mFrequency.QuadPart;
	}

	f64 Win32Time::GetInSecImpl()
	{
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;
		return static_cast<double>(elapsed.QuadPart) / mFrequency.QuadPart;
	}

	const Time Win32Time::GetDateTimeOS()
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
}