#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Color.h"
#include "Oniun/Platform/FileSystem.h"

using namespace Onu;

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>(TEXT("OutputFile.txt")));

    LOG(Info, TEXT("Color: {}"), DarkRed);
    LOG(Info, TEXT("Path: {}"), FileSystem::GetSpecialDirectoryPath(SpecialDirectory::Desktop));
    LOG(Info, TEXT("Path: {}"), FileSystem::GetSpecialDirectoryPath(SpecialDirectory::Downloads));
    LOG(Info, TEXT("Path: {}"), FileSystem::GetSpecialDirectoryPath(SpecialDirectory::AppData));
    LOG(Info, TEXT("Path: {}"), FileSystem::GetSpecialDirectoryPath(SpecialDirectory::LocalAppData));

    return 0;
}
