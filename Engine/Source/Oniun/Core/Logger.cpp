#include "Oniun/Core/Logger.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Time/DateTime.h"
#include "Oniun/Platform/Platform.h"

StringView ToString(LogType type)
{
    switch (type)
    {
    case LogType::Verbose:
        return "Verbose";
    case LogType::Trace:
        return "Trace";
    case LogType::Info:
        return "Info";
    case LogType::Warning:
        return "Warning";
    case LogType::Error:
        return "Error";
    case LogType::Fatal:
        return "Fatal";
    default:
        return "Invalid";
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
    uint64 engineIndex = file.FindLast("Oniun");
    if (engineIndex != GlobalVars::NoPos)
    {
        path.Set(ToSlice(file.begin() + engineIndex, file.end()));
    }
    path.CorrectPathSlashes();

    DateTime time(DateTime::Now());
    String message = Format("[{} {} {}:{} {}]:\n{}\n", type, time, function, line, path, userMsg);

    for (ILogOutput* output : m_Outputs)
        output->Write(type, message, file, function, line, userMsg, time);

    if (type == LogType::Fatal)
        std::exit(-1);
}

TerminalLogOutput::TerminalLogOutput()
    : ILogOutput("Terminal Output"), m_StdStream(Platform::GetStdOutStream()),
      m_ErrorStream(Platform::GetStdOutStream())
{
}

TerminalLogOutput::~TerminalLogOutput()
{
}

void TerminalLogOutput::Write(LogType type, const StringView& message, const StringView& file,
                              const StringView& function, int32 line, const StringView& userMsg,
                              const DateTime& time)
{
    File* stream = type > LogType::Info ? &m_ErrorStream : &m_StdStream;
    stream->Write(message.Data(), static_cast<uint32>(message.Length()));
#if !defined(ONU_PLATFORM_WINDOWS)
        stream->Flush();
#endif
}

FileLogOutput::FileLogOutput(const StringView& outputPath)
    : ILogOutput("Log File Output"), m_Output(outputPath, FileAccess::Write)
{
    uint32 size = m_Output.GetSize();
    m_Output.SetPosition(size);
}

FileLogOutput::~FileLogOutput()
{
}

void FileLogOutput::Write(LogType type, const StringView& message, const StringView& file,
                          const StringView& function, int32 line, const StringView& userMsg,
                          const DateTime& time)
{
    m_Output.Write(message.Data(), static_cast<uint32>(message.Length()));
    m_Output.Flush();
}
