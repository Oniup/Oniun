#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/PLatform/EntryPoint.h"
#include "Oniun/Renderer/RendererLayer.h"

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
    Engine::RegisterLayer<RendererLayer>(Format("{} ({})", info.Name, info.EngineBuild), -1, -1, Window::DefaultFlags);

    ImGuiLayer* imGui = Engine::RegisterLayer<ImGuiLayer>();
    imGui->Add(Memory::Allocate<DockingSpace>());
    imGui->Add(Memory::Allocate<Console>());
    imGui->Add(Memory::Allocate<Hierarchy>());

    Engine::Run();
    Engine::Terminate();

    return 0;
}
