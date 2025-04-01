#pragma once

#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    class DateTime
    {
    public:
        static DateTime Now();

    public:
        DateTime();
        DateTime(int64 time);
        DateTime(int32 seconds, int32 minutes, int32 hour, int32 month, int32 monthDay, int32 year);

    public:
        int64 GetTime() const;

        int32 GetSeconds() const;
        int32 GetMinutes() const;
        int32 GetHour(bool militaryTime = false) const;

        int32 GetMonth() const;
        int32 GetMonthDay() const;

        int32 GetYear() const;
        int32 GetYearDay() const;

    private:
        int64 m_Time;
    };

    template <>
    struct Formatter<DateTime>
    {
        bool IncludeTime = true;
        bool IncludeDate = true;
        bool MilitaryTime = false;

        bool Parse(const FormatArgsContext& context);
        void FormatTo(String& dest, const DateTime& dateTime);
    };
}
