#pragma once

#include "Oniun/Core/ClassConstraints.h"
#include "Oniun/Core/DateTime.h"
#include "Oniun/Core/String/Format.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Platform/File.h"

namespace Onu
{
    enum class LogType
    {
        Verbose,
        Trace,
        Info,
        Warning,
        Error,
        Fatal,
    };

    StringView ToString(LogType type);

    class ILogOutput
    {
    private:
        String m_Name;

    public:
        ILogOutput(const StringView& name)
            : m_Name(name)
        {
        }

        virtual ~ILogOutput()
        {
        }

        virtual void Write(LogType type, const CharStringView& message)
        {
        }

        virtual void Write(LogType type, const StringView& file, const StringView& function, int32 line, const StringView& message, const DateTime& time)
        {
        }
    };

    class Logger : public Singleton<Logger>
    {
    private:
        Array<ILogOutput*> m_Outputs;

    public:
        Logger();
        ~Logger();

        static void Write(LogType type, const StringView& file, const StringView& function, int32 line, const StringView& format)
        {
            Instance()->WriteImpl(type, file, function, line, format);
        }

        template<typename... TArgs>
        static void Write(LogType type, const StringView& file, const StringView& function, int32 line, const StringView& format, const TArgs&... args)
        {
            String message = Format(format, args...);
            Instance()->WriteImpl(type, file, function, line, message);
        }

        static void AddOutput(ILogOutput* entry);

    private:
        void WriteImpl(LogType type, const StringView& file, const StringView& function, int32 line, const StringView& message);
    };

    class TerminalLogOutput : public ILogOutput
    {
    private:
        File m_StdStream;
        File m_ErrorStream;

    public:
        TerminalLogOutput();
        ~TerminalLogOutput() override;

        void Write(LogType type, const CharStringView& message) override;
    };

    class FileLogOutput : public ILogOutput
    {
    private:
        File m_Output;

    public:
        FileLogOutput(const StringView& outputPath);
        ~FileLogOutput() override;

        void Write(LogType type, const CharStringView& message) override;
    };
}

#define LOG(_Type, _Format, ...) \
    Onu::Logger::Write(Onu::LogType::_Type, __FILEW__, __FUNCTIONW__, __LINE__, _Format, __VA_ARGS__)
