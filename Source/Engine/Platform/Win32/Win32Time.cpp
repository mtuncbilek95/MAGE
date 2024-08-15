#include "Win32Time.h"

#include <Windows.h>

namespace MAGE
{
	Time Win32Time::CurrentDateTime()
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
