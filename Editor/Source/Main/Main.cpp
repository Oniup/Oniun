#include "Oniun/Core/Logger.h"
#include "Oniun/Platform/FileSystem.h"

using namespace Onu;

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>(TEXT("OutputFile.txt")));

    Array<String> files;
    FileSystem::GetDirectoryFiles(files, TEXT("TestDir"), TEXT("*"), DirectorySearch::All);
    LOG(Info, TEXT("Files found: {}"), files);

    String absolute(FileSystem::RelativePathToAbsolute(TEXT("TestDir")));
    String relative(FileSystem::AbsolutePathToRelative(absolute));
    LOG(Info, TEXT("Absolute path: {}, Relative path: {}"), absolute, relative);

    return 0;
}
