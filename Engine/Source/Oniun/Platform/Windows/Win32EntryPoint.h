#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Engine.h"

namespace Oniun
{
    int EntryPoint(const CommandLineArguments& args);
}

#if ONU_PLATFORM_WINDOWS && ONU_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
{
    return Oniun::EntryPoint(Oniun::CommandLineArguments{__argc, __argv});
}

#else

int main(int argc, char** argv)
{
    return Oniun::EntryPoint(Oniun::CommandLineArguments{argc, argv});
}

#endif
