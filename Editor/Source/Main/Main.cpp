#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/PLatform/EntryPoint.h"
#include "Oniun/Renderer/Renderer.h"

int EntryPoint(int argc, char** argv)
{
#if !ONU_DIST
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
#endif
    Logger::AddOutput(Memory::Allocate<FileLogOutput>("OutputFile.txt"));

    Engine::Initialize(AppInfo{
        .Name = "Oniun Engine",
        .AppBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        .EngineBuild =  AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
    });

    const AppInfo& info = Engine::GetAppInfo();
    Renderer* renderer = Engine::RegisterLayer<Renderer>(Format("{} ({})", info.Name, info.EngineBuild), -1, -1, Window::DefaultFlags);
    ImGuiWindowManager* guiManager = renderer->GetImGuiWindowManager();

    guiManager->Add(Memory::Allocate<DockingSpace>());
    guiManager->Add(Memory::Allocate<Console>());
    guiManager->Add(Memory::Allocate<Hierarchy>());

    Engine::Run();
    Engine::Terminate();

    return 0;
}
