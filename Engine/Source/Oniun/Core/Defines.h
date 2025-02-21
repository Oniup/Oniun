#pragma once

// TODO: Remove when implementing debug logger
#include <cstdio>
#include <cstdlib>

namespace Onu
{
    enum PlatformType
    {
        PlatformType_Windows,
        PlatformType_Macos,
        PlatformType_Linux,

        PlatformType_Ios,
        PlatformType_Android,

        PlatformType_XboxOne,
        PlatformType_XboxScarlet,

        PlatformType_Ps4,
        PlatformType_Ps5,

        PlatformType_Switch,
    };

    enum class ArchitectureType
    {
        ArchitectureType_X64,
        ArchitectureType_Arm64,
    };

    // TODO: Move to debug logger
    inline void Assert(const char* expression, const char* file, const char* function, int line)
    {
        std::fprintf(stderr, "Fatal: (%s) At %s:%d in %s", expression, function, line, file);
        std::exit(-1);
    }
}

// Platform

#if !defined(ONU_PLATFORM_WINDOWS)
#define ONU_PLATFORM_WINDOWS 0
#else
#undef ONU_PLATFORM_WINDOWS
#define ONU_PLATFORM_WINDOWS 1
#endif

#if !defined(ONU_PLATFORM_LINUX)
#define ONU_PLATFORM_LINUX 0
#else
#undef ONU_PLATFORM_LINUX
#define ONU_PLATFORM_LINUX 1
#endif

#if !defined(ONU_PLATFORM_MACOS)
#define ONU_PLATFORM_MACOS 0
#else
#undef ONU_PLATFORM_MACOS
#define ONU_PLATFORM_MACOS 1
#endif

// Build

#if !defined(ONU_DEBUG)
#define ONU_DEBUG 0
#else
#undef ONU_DEBUG
#define ONU_DEBUG 1
#endif

#if !defined(ONU_RELEASE)
#define ONU_RELEASE 0
#else
#undef ONU_RELEASE
#define ONU_RELEASE 1
#endif

#if !defined(ONU_DIST)
#define ONU_DIST 0
#else
#undef ONU_DIST
#define ONU_DIST 1
#endif

// Unicode text support

#if !defined(TEXT)
#ifdef ONU_PLATFORM_WINDOWS
#define INTL_TEXT(_X) L ## _X
#else
#define INTL_TEXT(_X) u ## _X
#endif
#define TEXT(_X) INTL_TEXT(_X)
#endif


#define DEFAULT_HASH_MAP_CAPACITY_COUNT 256
#define CARRAY_COUNT(_CArray) (sizeof(_CArray) / sizeof(_CArray[0])

#if !ONU_DIST
#define ASSERT(_Expression)\
{\
    if (!(_Expression))\
        Onu::Assert(#_Expression, __FILE__, __FUNCTION__, __LINE__);\
}
#endif
