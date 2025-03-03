#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32EntryPoint.h"

#elif ONU_PLATFORM_UNIX

#include "Oniun/Platform/Unix/UnixEntryPoint.h"

#elif ONU_PLATFORM_LINUX

#include "Oniun/Platform/Linux/LinuxEntryPoint.h"

#elif ONU_PLATFORM_MACOS

#include "Oniun/Platform/Macos/MacosEntryPoint.h"

#endif
