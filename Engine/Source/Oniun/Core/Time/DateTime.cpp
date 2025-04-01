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
        ASSERT(m_Time != -1)
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

    bool Formatter<DateTime>::Parse(const FormatArgsContext& context)
    {
        for (StringView arg : context)
        {
            if (arg == "m")
                MilitaryTime = true;
            else if (arg == "rt")
                IncludeTime = false;
            else if (arg == "rd")
                IncludeDate = false;
        }
        return true;
    }

    void Formatter<DateTime>::FormatTo(String& dest, const DateTime& dateTime)
    {
        ASSERT((IncludeTime && IncludeDate) || (IncludeTime || IncludeDate))

        time_t time = dateTime.GetTime();
        tm* tm = localtime(&time);

        if (IncludeTime)
            Fmt::FormatTo(dest, "{}:{}:{}", dateTime.GetHour(MilitaryTime), tm->tm_min, tm->tm_sec);

        if (IncludeDate)
        {
            if (IncludeTime)
                dest.Append(' ');
            Fmt::FormatTo(dest, "{}/{}/{}", tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900);
        }
    }
}
