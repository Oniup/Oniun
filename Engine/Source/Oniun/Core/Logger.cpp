#include "Oniun/Core/Logger.h"

#include "Oniun/Core/Time/DateTime.h"
#include "Oniun/Platform/Platform.h"

namespace Oniun
{
    Logger* Logger::m_Instance = nullptr;

    Logger::Logger()
    {
        m_Instance = this;
    }

    Logger::~Logger()
    {
        for (ILogOutput* output : m_Outputs)
            Memory::Delete(output);
        m_Instance = nullptr;
    }

    void Logger::AddOutput(ILogOutput* entry)
    {
        m_Instance->m_Outputs.Add(entry);
    }

    void Logger::RemoveOutput(const StringView& name)
    {
        Array<ILogOutput*> output = m_Instance->m_Outputs;
        for (uint64 i = 0; i < output.Count(); ++i)
        {
            if (output[i]->GetName() == name)
            {
                output.RemoveAt(i);
                return;
            }
        }
    }

    ILogOutput* Logger::GetOutput(const StringView& name)
    {
        for (ILogOutput* output : m_Instance->m_Outputs)
        {
            if (output->GetName() == name)
                return output;
        }
        return nullptr;
    }

    void Logger::WriteImpl(LogType type, const StringView& file, const StringView& function, int32 line, const StringView& userMessage)
    {
        String path(file);
        uint64 index = file.Find("Oniun");
        if (index != NO_POS)
            path.Set(ToSlice(file.Begin() + index, file.End()));
        path.CorrectPathSlashes();

        DateTime time(DateTime::Now());
        String formattedMessage = Fmt::Format("[{} {} {}:{} {}]:\n{}\n", type, time, function, line, path, userMessage);

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

    void TerminalLogOutput::Write(LogType type, const StringView& formattedMessage, const StringView& file, const StringView& function, int32 line, const StringView& userMessage, const DateTime& time)
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
        if (m_Output.IsOpen())
        {
            uint32 size = m_Output.GetSize();
            m_Output.SetPosition(size);
            m_Path = outputPath;
        }
    }

    FileLogOutput::~FileLogOutput()
    {
    }

    void FileLogOutput::Write(LogType type, const StringView& formattedMessage, const StringView& file, const StringView& function, int32 line, const StringView& userMessage, const DateTime& time)
    {
        m_Output.Write(formattedMessage.Data(), static_cast<uint32>(formattedMessage.Length()));
        m_Output.Flush();
    }

    void FileLogOutput::SetPath(const StringView& path)
    {
        File newOutput(path, FileAccess::Write);
        if (newOutput.IsOpen())
        {
            m_Path = path;
            m_Output = Memory::Move(newOutput);
        }
    }

    bool Formatter<LogType>::Parse(const FormatArgsContext& ctx)
    {
        return true;
    }

    void Formatter<LogType>::FormatTo(String& dest, LogType type)
    {
        switch (type)
        {
        case LogType::Verbose:
            dest.Append("Verbose");
            break;
        case LogType::Trace:
            dest.Append("Trace");
            break;
        case LogType::Info:
            dest.Append("Info");
            break;
        case LogType::Warning:
            dest.Append("Warning");
            break;
        case LogType::Error:
            dest.Append("Error");
            break;
        case LogType::Fatal:
            dest.Append("Fatal");
            break;
        default:
            dest.Append("Invalid");
        }
    }
}
