#include "LinuxTime.h"

#include <ctime>
#include <unistd.h>
#include <sys/time.h>

namespace MAGE {
    Time LinuxTime::CurrentDateTime() {
        timespec ts{};
        clock_gettime(CLOCK_REALTIME, &ts);
        tm timeMap{};
        localtime_r(&ts.tv_sec, &timeMap);

        Time time{};
        time.Day = timeMap.tm_mday;
        time.Month = timeMap.tm_mon + 1;
        time.Year = timeMap.tm_year + 1900;
        time.Hour = timeMap.tm_hour;
        time.Minute = timeMap.tm_min;
        time.Second = timeMap.tm_sec;
        time.Millisecond = ts.tv_nsec / 1000000;
        return time;
    }
}
