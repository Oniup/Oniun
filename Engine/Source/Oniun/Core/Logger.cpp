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

void Logger::RemoveOutput(const StringView& name)
{
    Array<ILogOutput*>& outputs = Instance()->m_Outputs;
    for (uint64 i = 0; i < outputs.Count(); ++i)
    {
         if (outputs[i]->GetName() == name)
         {
             outputs.RemoveAt(i);
             return;
         }
    }
}

void Logger::WriteImpl(LogType type, const StringView& file, const StringView& function, int32 line,
                       const StringView& userMessage)
{
    String path(file);
    uint64 index = file.Find("Oniun");
    if (index != INVALID_INDEX)
        path.Set(ToSlice(file.Begin() + index, file.End()));
    path.CorrectPathSlashes();

    DateTime time(DateTime::Now());
    String formattedMessage = Format("[{} {} {}:{} {}]:\n{}\n", type, time, function, line, path, userMessage);

    for (ILogOutput* output : m_Outputs)
        output->Write(type, formattedMessage, path, function, line, userMessage, time);

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

void TerminalLogOutput::Write(LogType type, const StringView& formattedMessage, const StringView& file,
                              const StringView& function, int32 line, const StringView& userMessage,
                              const DateTime& time)
{
    File* stream = type > LogType::Info ? &m_ErrorStream : &m_StdStream;
    stream->Write(formattedMessage.Data(), static_cast<uint32>(formattedMessage.Length()));
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

void FileLogOutput::Write(LogType type, const StringView& formattedMessage, const StringView& file,
                          const StringView& function, int32 line, const StringView& userMessage,
                          const DateTime& time)
{
    m_Output.Write(formattedMessage.Data(), static_cast<uint32>(formattedMessage.Length()));
    m_Output.Flush();
}
