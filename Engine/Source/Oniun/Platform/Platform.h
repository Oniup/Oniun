#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Win32/Win32Platform.h"

#elif ONU_PLATFORM_UNIX

#include "Oniun/Platform/Unix/UnixPlatform.h"

#elif ONU_PLATFORM_LINUX

#include "Oniun/Platform/Linux/LinuxPlatform.h"

#elif ONU_PLATFORM_MACOS

#include "Oniun/Platform/Macos/MacosPlatform.h"

#endif
