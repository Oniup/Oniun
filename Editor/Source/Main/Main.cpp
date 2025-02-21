#include <cwchar>

#include <Oniun/Core/Logger.h>
#include <Oniun/Core/Memory/Memory.h>
#include "Main/Tests.h"

using namespace Onu;

int main()
{
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
    Logger::AddOutput(Memory::Allocate<FileLogOutput>(TEXT("OutputFile.txt")));

    LOG(Info, TEXT("Normal info log, nothing major"));
    LOG(Warning, TEXT("Should deal with this warning, could lead to issues later on or become a much larger issue"));
    LOG(Error, TEXT("Need to fix this issue as soon as possible. This is not outputting expected results"));

    return 0;
}
