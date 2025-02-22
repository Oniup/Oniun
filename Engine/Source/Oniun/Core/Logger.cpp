#include "Oniun/Core/Logger.h"

#include "Oniun/Core/DateTime.h"
#include "Oniun/Core/Engine.h"
#include "Oniun/Platform/Platform.h"

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"

namespace Onu
{
    StringView AnsiColor(LogType type)
    {
        switch (type)
        {
        case LogType::Warning:
            TEXT(YELLOW);
        case LogType::Error:
        case LogType::Fatal:
            TEXT(RED);
        default:
            return TEXT(RESET);
        }
    }

    StringView ToString(LogType type)
    {
        switch (type)
        {
        case LogType::Verbose:
            return TEXT("Verbose");
        case LogType::Trace:
            return TEXT("Trace");
        case LogType::Info:
            return TEXT("Info");
        case LogType::Warning:
            return TEXT("Warning");
        case LogType::Error:
            return TEXT("Error");
        case LogType::Fatal:
            return TEXT("Fatal");
        default:
            return TEXT("Invalid");
        }
    }

    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
        for (ILogOutput* output : m_Outputs)
            Memory::Free(output);
    }

    void Logger::AddOutput(ILogOutput* entry)
    {
        Instance()->m_Outputs.Add(entry);
    }

    void Logger::WriteImpl(LogType type, const StringView& file, const StringView& function, int32 line,
                           const StringView& userMsg)
    {
        DateTime time(DateTime::Now());
        String utf16Message = Format(TEXT("[{} {} {}:{}:{}]:\n{}\n"), type, time, file, function, line, userMsg);
        CharString utf8Message(StringUtils::Utf16ToUtf8(utf16Message));

        for (ILogOutput* output : m_Outputs)
            output->Write(type, utf8Message, file, function, line, userMsg, time);
    }

    TerminalLogOutput::TerminalLogOutput()
        : ILogOutput(TEXT("Terminal Output")), m_StdStream(Platform::GetStdOutStream()),
          m_ErrorStream(Platform::GetStdOutStream())
    {
    }

    TerminalLogOutput::~TerminalLogOutput()
    {
    }

    void TerminalLogOutput::Write(LogType type, const CharStringView& utf8FinalMsg, const StringView& file,
                                  const StringView& function, int32 line, const StringView& userMsg,
                                  const DateTime& time)
    {
        File* stream = type > LogType::Info ? &m_ErrorStream : &m_StdStream;
        stream->Write(utf8FinalMsg.Data(), static_cast<uint32>(utf8FinalMsg.Length()));
        stream->Flush();
    }

    FileLogOutput::FileLogOutput(const StringView& outputPath)
        : ILogOutput(TEXT("Log File Output")), m_Output(outputPath, FileAccess::Write)
    {
        uint32 size = m_Output.GetSize();
        m_Output.SetPosition(size);
    }

    FileLogOutput::~FileLogOutput()
    {
    }

    void FileLogOutput::Write(LogType type, const CharStringView& utf8FinalMsg, const StringView& file,
                              const StringView& function, int32 line, const StringView& userMsg,
                              const DateTime& time)
    {
        m_Output.Write(utf8FinalMsg.Data(), static_cast<uint32>(utf8FinalMsg.Length()));
        m_Output.Flush();
    }
}
