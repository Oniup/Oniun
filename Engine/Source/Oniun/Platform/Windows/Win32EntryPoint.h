#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Engine.h"

int EntryPoint(const CommandLineArguments& args);

#if ONU_PLATFORM_WINDOWS && ONU_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
{
    return EntryPoint(CommandLineArguments{__argc, __argv});
}

#else

int main(int argc, char** argv)
{
    return EntryPoint(CommandLineArguments{argc, argv});
}

#endif
