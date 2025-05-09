#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32File.h"

#elif ONU_PLATFORM_UNIX

#include "Oniun/Platform/Unix/UnixFile.h"

#elif ONU_PLATFORM_LINUX

#include "Oniun/Platform/Linux/LinuxFile.h"

#elif ONU_PLATFORM_MACOS

#include "Oniun/Platform/Macos/MacosFile.h"

#endif
