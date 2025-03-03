#pragma once

#include "Oniun/Core/ClassConstraints.h"
#include "Oniun/Core/String/Format.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Platform/File.h"

enum class LogType : uint64
{
    Verbose,
    Trace,
    Info,
    Warning,
    Error,
    Fatal,
    Count,
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

    FORCE_INLINE String GetName() const
    {
        return m_Name;
    }

    virtual void Write(LogType type, const StringView& formattedMessage, const StringView& file,
                       const StringView& function, int32 line, const StringView& userMessage,
                       const DateTime& time) = 0;
};

class Logger : public Singleton<Logger>
{
private:
    Array<ILogOutput*> m_Outputs;

public:
    Logger();
    ~Logger();

    static void Write(LogType type, const StringView& file, const StringView& function, int32 line,
                      const StringView& format)
    {
        Instance()->WriteImpl(type, file, function, line, format);
    }

    template <typename... TArgs>
    static void Write(LogType type, const StringView& file, const StringView& function, int32 line,
                      const StringView& format, const TArgs&... args)
    {
        String usrMsg = Format(format, args...);
        Instance()->WriteImpl(type, file, function, line, usrMsg);
    }

    static void AddOutput(ILogOutput* entry);
    static void RemoveOutput(const StringView& name);

private:
    void WriteImpl(LogType type, const StringView& file, const StringView& function, int32 line,
                   const StringView& userMessage);
};

class TerminalLogOutput : public ILogOutput
{
private:
    File m_StdStream;
    File m_ErrorStream;

public:
    TerminalLogOutput();
    ~TerminalLogOutput() override;

    void Write(LogType type, const StringView& formattedMessage, const StringView& file, const StringView& function,
               int32 line, const StringView& userMessage, const DateTime& time) override;
};

class FileLogOutput : public ILogOutput
{
private:
    File m_Output;

public:
    FileLogOutput(const StringView& outputPath);
    ~FileLogOutput() override;

    void Write(LogType type, const StringView& formattedMessage, const StringView& file, const StringView& function,
               int32 line, const StringView& userMessage, const DateTime& time) override;
};

#define LOG(_Type, _Format, ...) \
    Logger::Write(LogType::_Type, __FILE__, __FUNCTION__, __LINE__, _Format, __VA_ARGS__)
