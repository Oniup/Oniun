#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Core/BaseTypes.h"

int EntryPoint(int32 argc, char** argv);

#if ONU_DIST

#include <Windows.h>

inline int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
{
    return EntryPoint(__argc, __argv);
}

#else

inline int main(int argc, char** argv)
{
    return EntryPoint(argc, argv);
}

#endif

#endif
