#pragma once

#include "Oniun/Core/Engine.h"

#if ONU_PLATFORM_WINDOWS && NDEBUG

#include <Windows.h>
#include "Oniun/Platform/Windows/Win32Headers.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
{
    Oniun::Engine* engine = Oniun::CreateApplication(Oniun::CommandLineArguments(__argc, __argv));
    engine->Setup();
    engine->Run();
    Oniun::Memory::Delete(engine);
    return 0;
}

#else

int main(int argc, char** argv)
{
    Oniun::Engine* engine = Oniun::CreateApplication(Oniun::CommandLineArguments(__argc, __argv));
    engine->Setup();
    engine->Run();
    Oniun::Memory::Delete(engine);
    return 0;
}

#endif
