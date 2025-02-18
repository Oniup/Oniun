#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Win32/Win32File.h"

#elif ONU_PLATFORM_UNIX

#include "Oniun/Platform/Unix/FileStream.h"

#elif ONU_PLATFORM_LINUX

#include "Oniun/Platform/Linux/FileStream.h"

#elif ONU_PLATFORM_MACOS

#include "Oniun/Platform/Macos/FileStream.h"

#endif
