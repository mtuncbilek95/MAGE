#include "DayTime.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace MAGE
{
	const array<string, 12> monthsInString = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	string DayTime::GetMonthName() const
	{
		return monthsInString[m_month - 1];
	}

	DayTime DayTime::GetCurrentDayTime()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

		std::tm localTime;
		localtime_s(&localTime, &currentTime);

		DayTime currentDayTime;
		currentDayTime.m_year = 1900 + localTime.tm_year;
		currentDayTime.m_month = 1 + localTime.tm_mon;
		currentDayTime.m_day = localTime.tm_mday;
		currentDayTime.m_hour = localTime.tm_hour;
		currentDayTime.m_minute = localTime.tm_min;
		currentDayTime.m_second = localTime.tm_sec;

		return currentDayTime;
	}

	string DayTime::ToString() const
	{
		char buffer[64];
		std::snprintf(buffer, sizeof(buffer), "%02d/%02d/%d %02d:%02d:%02d",
			m_month, m_day, m_year, m_hour, m_minute, m_second);

		return string(buffer);
	}
}
