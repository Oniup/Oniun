#pragma once

#include "Oniun/Core/BaseTypes.h"

int EntryPoint(int32 argc, char** argv);

#if ONU_PLATFORM_WINDOWS && ONU_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
{
    return EntryPoint(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
    return EntryPoint(argc, argv);
}

#endif
