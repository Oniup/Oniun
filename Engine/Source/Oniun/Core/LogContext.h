#pragma once

#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Platform/File.h"

namespace Onu
{
    // enum LogSeverity
    // {
    //     LogSeverity_None = 0,
    //     LogSeverity_Trace,
    //     LogSeverity_Verbose,
    //     LogSeverity_Info,
    //     LogSeverity_Warning,
    //     LogSeverity_Error,
    //     LogSeverity_Fatal,
    // };
    //
    // class ILogger : public INonCopyable
    // {
    // public:
    //     virtual ~ILogger()
    //     {
    //     }
    //
    //     void LogMessage(const StringView& file, const StringView& function, int32 line, LogSeverity severity,
    //                     const String& message);
    //
    // protected:
    //     virtual bool FormatMessage(String& result, const StringView& file, const StringView& function, int32 line,
    //                                LogSeverity severity, const String& message) = 0;
    //     virtual bool PrintToOutput(const StringView& file, const StringView& function, int32 line, LogSeverity severity,
    //                                const String& message) = 0;
    // };
    //
    // class TerminalLogger : public ILogger
    // {
    // protected:
    //     bool FormatMessage(String& result, const StringView& file, const StringView& function, int32 line,
    //                        LogSeverity severity, const String& message) override;
    //     bool PrintToOutput(const StringView& file, const StringView& function, int32 line, LogSeverity severity,
    //                        const String& message) override;
    // };
    //
    // class LogContext
    // {
    // private:
    //     Array<ILogger*> m_Loggers;
    //
    // public:
    //     static void LogMessage(const StringView& file, const StringView& function, int32 line, LogSeverity severity,
    //                            const String& message);
    //
    //     LogContext();
    //     ~LogContext();
    //
    //     void AddLogger(ILogger* logger);
    // };
}
