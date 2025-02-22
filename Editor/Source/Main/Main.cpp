#include "Oniun/Core/Logger.h"
#include "Oniun/Platform/FileSystem.h"

using namespace Onu;

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>(TEXT("OutputFile.txt")));

    LOG(Info, TEXT("Current working directory: {}"), FileSystem::GetCurrentWorkingDirectory());

    return 0;
}
