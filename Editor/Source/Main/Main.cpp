#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Color.h"
#include "Oniun/Platform/FileSystem.h"

using namespace Onu;

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>(TEXT("OutputFile.txt")));

    return 0;
}
