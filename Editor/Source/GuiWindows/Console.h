#pragma once

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Time/DateTime.h"
#include "Oniun/RHI/IImGuiWindow.h"

class Console : public IImGuiWindow
{
private:
    bool m_Colored = true;
    bool m_IncludeFilter[(uint64)LogType::Count];

public:
    struct Output
    {
        LogType Type;
        String File;
        StringView Function;
        String UserMessage;
        int32 Line;
        DateTime Time;
    };

private:
    Array<Output> m_Logs;

public:
    Console();
    ~Console() override;

    void AddLog(Output&& log);

    void Draw() override;

    void DrawOutputLogs();
    void DrawMenuBar();
};

class ConsoleLogOutput : public ILogOutput
{
private:
    Console* m_Console;

public:
    ConsoleLogOutput(Console* console);

    void Write(LogType type, const StringView& formattedMessage, const StringView& file, const StringView& function,
               int32 line, const StringView& userMessage, const DateTime& time) override;
};
