#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Renderer/Renderer.h"

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>(TEXT("OutputFile.txt")));

    Engine::Initialize(AppInfo{
        .Name = TEXT("Oniun Engine"),
        .AppBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        .EngineBuild =  AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
    });

    Engine::RegisterLayer<Renderer>("Test Window", -1, -1);
    Engine::Run();

    return 0;
}
