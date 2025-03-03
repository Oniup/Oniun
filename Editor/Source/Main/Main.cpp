#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"
#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Renderer/Renderer.h"

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>("OutputFile.txt"));

    Engine::Initialize(AppInfo{
        .Name = "Oniun Engine",
        .AppBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        .EngineBuild =  AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
    });

    Renderer* renderer = Engine::RegisterLayer<Renderer>("Test Window", -1, -1);
    ImGuiWindowManager* guiManager = renderer->GetImGuiWindowManager();

    guiManager->Add(Memory::Allocate<DockingSpace>());
    guiManager->Add(Memory::Allocate<Hierarchy>());

    Engine::Run();
    Engine::Terminate();

    return 0;
}
