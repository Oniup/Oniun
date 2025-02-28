#include "Oniun/Core/Logger.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Time/DateTime.h"
#include "Oniun/Platform/Platform.h"

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
    String path(file);
    uint64 engineIndex = file.FindLast(TEXT("Oniun"));
    if (engineIndex != GlobalVars::NoPos)
    {
        path.Set(ToSlice(file.begin() + engineIndex, file.end()));
    }
    path.CorrectPathSlashes();

    DateTime time(DateTime::Now());
    String utf16Message = Format(TEXT("[{} {} {}:{} {}]:\n{}\n"), type, time, function, line, path, userMsg);
    CharString utf8Message(StringUtils::Utf16ToUtf8(utf16Message));

    for (ILogOutput* output : m_Outputs)
        output->Write(type, utf8Message, file, function, line, userMsg, time);

    if (type == LogType::Fatal)
        std::exit(-1);
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
#if !defined(ONU_PLATFORM_WINDOWS)
        stream->Flush();
#endif
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
