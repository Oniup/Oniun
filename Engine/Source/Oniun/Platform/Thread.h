#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32Thread.h"

#elif ONU_PLATFORM_UNIX

#include "Oniun/Platform/Unix/UnixThread.h"

#elif ONU_PLATFORM_LINUX

#include "Oniun/Platform/Linux/LinuxThread.h"

#elif ONU_PLATFORM_MACOS

#include "Oniun/Platform/Macos/MacosThread.h"

#endif
