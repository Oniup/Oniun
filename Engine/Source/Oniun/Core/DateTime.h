#pragma once

#include "Oniun/Core/BaseTypes.h"

namespace Onu
{
    class DateTime
    {
        time_t m_Time;

    public:
        static DateTime Now();

        DateTime();
        DateTime(time_t time);
        DateTime(int32 seconds, int32 minutes, int32 hour, int32 month, int32 monthDay, int32 year);

        time_t GetTime() const;

        int32 GetSeconds() const;
        int32 GetMinutes() const;
        int32 GetHour(bool militaryTime = false) const;

        int32 GetMonth() const;
        int32 GetMonthDay() const;

        int32 GetYear() const;
        int32 GetYearDay() const;
    };

    String ToString(const DateTime& dateTime, bool includeTime = true, bool includeDate = true,
                    bool militaryTime = false);
}
