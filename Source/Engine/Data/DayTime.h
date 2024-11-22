/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	class DayTime final
	{
	public:
		DayTime() = default;
		~DayTime() = default;

		DayTime operator=(const DayTime& other)
		{
			if (this != &other) {
				m_month = other.m_month;
				m_day = other.m_day;
				m_year = other.m_year;
				m_hour = other.m_hour;
				m_minute = other.m_minute;
				m_second = other.m_second;
			}
			return *this;
		}

		u8 GetMonth() const { return m_month; }
		string GetMonthName() const;
		u8 GetDay() const { return m_day; }
		u16 GetYear() const { return m_year; }
		u8 GetHour() const { return m_hour; }
		u8 GetMinute() const { return m_minute; }
		u8 GetSecond() const { return m_second; }

		static DayTime GetCurrentTime();

		string ToString() const;

		b8 operator==(const DayTime& other) const {
			return m_month == other.m_month &&
				m_day == other.m_day &&
				m_year == other.m_year &&
				m_hour == other.m_hour &&
				m_minute == other.m_minute &&
				m_second == other.m_second;
		}

		b8 operator!=(const DayTime& other) const {
			return !(*this == other);
		}

	private:
		u8 m_month;
		u8 m_day;
		u16 m_year;
		u8 m_hour;
		u8 m_minute;
		u8 m_second;
	};
}
