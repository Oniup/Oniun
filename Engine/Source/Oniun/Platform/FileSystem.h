#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32FileSystem.h"

#elif ONU_PLATFORM_UNIX

#include "Oniun/Platform/Unix/UnixFileSystem.h"

#elif ONU_PLATFORM_LINUX

#include "Oniun/Platform/Linux/LinuxFileSystem.h"

#elif ONU_PLATFORM_MACOS

#include "Oniun/Platform/Macos/MacosFileSystem.h"

#endif
