#include "Oniun.pch.h"
#include "Oniun/Core/Time/DateTime.h"

#include <time.h>
#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    DateTime DateTime::Now()
    {
        return DateTime(time(nullptr));
    }

    DateTime::DateTime()
        : m_Time(0)
    {
    }

    DateTime::DateTime(int64 time)
        : m_Time(time)
    {
    }

    DateTime::DateTime(int32 seconds, int32 minutes, int32 hour, int32 month, int32 monthDay, int32 year)
    {
        tm timeInfo = {};
        timeInfo.tm_sec = seconds;
        timeInfo.tm_min = minutes;
        timeInfo.tm_hour = hour;
        timeInfo.tm_mon = month;
        timeInfo.tm_mday = monthDay;
        timeInfo.tm_year = year;
        m_Time = mktime(&timeInfo);
        DEBUG_ASSERT(m_Time != -1)
    }

    int64 DateTime::GetTime() const
    {
        return m_Time;
    }

    int32 DateTime::GetSeconds() const
    {
        tm* time = localtime(&m_Time);
        return time->tm_sec;
    }

    int32 DateTime::GetMinutes() const
    {
        tm* time = localtime(&m_Time);
        return time->tm_min;
    }

    int32 DateTime::GetHour(bool militaryTime) const
    {
        tm* time = localtime(&m_Time);
        int32 hour = time->tm_hour;
        if (militaryTime)
            return hour;
        hour = hour % 12;
        return hour == 0 ? 12 : hour;
    }

    int32 DateTime::GetMonth() const
    {
        tm* time = localtime(&m_Time);
        return time->tm_mon + 1;
    }

    int32 DateTime::GetMonthDay() const
    {
        tm* time = localtime(&m_Time);
        return time->tm_mday;
    }

    int32 DateTime::GetYear() const
    {
        tm* time = localtime(&m_Time);
        return time->tm_year + 1900;
    }

    int32 DateTime::GetYearDay() const
    {
        tm* time = localtime(&m_Time);
        return time->tm_yday + 1;
    }

    String ToString(const DateTime& dateTime, bool includeTime, bool includeDate, bool militaryTime)
    {
        DEBUG_ASSERT((includeTime && includeDate) || (includeTime || includeDate))

        time_t time = dateTime.GetTime();
        tm* tm = localtime(&time);

        if (includeTime && includeDate)
            return Format("{}:{}:{} {}/{}/{}", dateTime.GetHour(militaryTime), tm->tm_min, tm->tm_sec,
                          tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900);
        if (includeTime)
            return Format("{}:{}:{}", dateTime.GetHour(militaryTime), tm->tm_min, tm->tm_sec);

        return Format("{}/{}/{}", tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900);
    }
}
