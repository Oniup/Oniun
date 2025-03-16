#include "GuiWindows/Console.h"

#include <cstdio>
#include <ctime>

#include "Oniun/Core/Math/Color.h"

// TODO: Remove the constants and serialize them into a preferences file
static constexpr Color32 ErrorColor("#e7452d");
static constexpr Color32 WarningColor("#ffbf3a");
static constexpr Color32 VerboseTraceColor("#a6a6a6");

ConsoleLogOutput::ConsoleLogOutput(Console* console)
    : ILogOutput("Console"), m_Console(console)
{
}

void ConsoleLogOutput::Write(LogType type, const StringView& formattedMessage, const StringView& file,
    const StringView& function, int32 line, const StringView& userMessage, const DateTime& time)
{
    m_Console->AddLog(Memory::Move(Console::Output{
        .Type = type,
        .File = file,
        .Function = function,
        .UserMessage = userMessage,
        .Line = line,
        .Time = time,
    }));
}

Console::Console()
    : IImGuiWindow("Console", DefaultFlags | ImGuiWindowFlags_MenuBar)
{
    Logger::AddOutput(Memory::New<ConsoleLogOutput>(this));

    for (uint64 i = 0; i < (uint64)LogType::Count; ++i)
        m_IncludeFilter[i] = true;
}

Console::~Console()
{
    Logger::RemoveOutput("Console");
}

void Console::AddLog(Output&& log)
{
    m_Logs.Add(Memory::Move(log));
}

void Console::Draw()
{
    DrawMenuBar();
    float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion"), ImVec2(0, -footerHeightToReserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar)
    {
        DrawOutputLogs();

        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0);
        ImGui::EndChild();
    }
}

void Console::DrawOutputLogs()
{
    // Get lighter background color to be 1.5 times lighter than the base background
    const ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 lighterBg = style.Colors[ImGuiCol_WindowBg];
    constexpr float lighterMulti = 1.5f;
    lighterBg = ImVec4(lighterBg.x * lighterMulti, lighterBg.y * lighterMulti, lighterBg.z * lighterMulti, lighterBg.w);

    // Set color
    uint64 i = 0;
    for (const Output& log : m_Logs)
    {
        if (!m_IncludeFilter[(uint64)log.Type])
            continue;

        if (m_Colored)
        {
            switch (log.Type)
            {
            case LogType::Verbose:
            case LogType::Trace:
                ImGui::PushStyleColor(ImGuiCol_Text, VerboseTraceColor.Base);
                break;
            case LogType::Warning:
                ImGui::PushStyleColor(ImGuiCol_Text, WarningColor.Base);
                break;
            case LogType::Error:
            case LogType::Fatal:
                ImGui::PushStyleColor(ImGuiCol_Text, ErrorColor.Base);
                break;
            default:
                break;
            }
        }

        // Set background of log child to be lighter in odd in the array
        bool enableLighterBg = i % 2 == 1;
        if (enableLighterBg)
            ImGui::PushStyleColor(ImGuiCol_ChildBg, lighterBg);

        constexpr uint64 bufferMaxCount = 256;
        char childName[bufferMaxCount];
        std::snprintf(childName, bufferMaxCount, "LogMessage%llu", i);
        ImGui::BeginChild(childName, ImVec2(0.0f, 0.0f), ImGuiChildFlags_AutoResizeY);
        {
            // Display log
            ImGui::TextWrapped("[%s %d:%d:%d %d/%d/%d] %s:%d %s", *ToString(log.Type), log.Time.GetHour(),
                               log.Time.GetMinutes(), log.Time.GetSeconds(), log.Time.GetMonth(),
                               log.Time.GetMonthDay(),
                               log.Time.GetYear(), *log.Function, log.Line, *log.File);
            ImGui::TextWrapped("%s", *log.UserMessage);

            // Remove style
            if (m_Colored && log.Type != LogType::Info)
                ImGui::PopStyleColor();
            if (enableLighterBg)
                ImGui::PopStyleColor();

            // Open context menu by right-clicking
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Copy"))
                    ImGui::SetClipboardText(*Format("[{} {}]: {}:{}: {}\n{}", ToString(log.Type), log.Time,
                                                    log.Function, log.Line, log.File, log.UserMessage));
                ImGui::EndPopup();
            }
        }
        ImGui::EndChild();

        ++i;
    }
}

void Console::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Console"))
        {
            if (ImGui::MenuItem("Clear"))
                m_Logs.Clear();
            if (ImGui::BeginMenu("Filter"))
            {
                for (uint64 i = 0; i < (uint64)LogType::Count; ++i)
                    ImGui::Checkbox(*ToString((LogType)i), &m_IncludeFilter[i]);
                ImGui::EndMenu();
            }
            ImGui::MenuItem("Auto-scroll", nullptr, &m_AutoScroll);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::MenuItem("Colored", nullptr, &m_Colored);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}
