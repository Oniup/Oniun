#pragma once

// Primitive limits

#define MIN_UINT8   0x00
#define	MIN_UINT16  0x0000
#define	MIN_UINT32  0x00000000
#define MIN_UINT64  0x0000000000000000
#define MIN_INT8    -128
#define MIN_INT16   -32768
#define MIN_INT32   -2147483648ll
#define MIN_INT64   -9223372036854775808

#define MAX_UINT8   0xff
#define MAX_UINT16  0xffff
#define MAX_UINT32  0xffffffff
#define MAX_UINT64  0xffffffffffffffff
#define MAX_INT8    127
#define MAX_INT16   32767
#define MAX_INT32   2147483647
#define MAX_INT64   9223372036854775807

#define MIN_FLOAT   -3.402823466e+38f
#define MIN_DOUBLE  -1.7976931348623158e+308

#define MAX_FLOAT   3.402823466e+38f
#define MAX_DOUBLE  1.7976931348623158e+308

#define NO_POS MAX_UINT64

namespace Oniun
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

    namespace Assert_Internal
    {
        void Assert(const char* expression, const char* file, const char* function, int line);
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

#define DEFAULT_HASH_MAP_CAPACITY_COUNT 256
#define DEFAULT_COMPONENT_POOL_CHUNK_PER_BLOCK_COUNT 256
#define CARRAY_COUNT(_CArray) (sizeof(_CArray) / sizeof(_CArray[0]))

#define COMP_POOL_MAX_TYPE_COUNT 5

/// Assert exists program if _Expression is false
///
/// @param _Expression If true, then continue program, otherwise exists out with exit code -1
#define FORCE_ASSERT(_Expression) \
{ \
    if (!(_Expression)) \
        Oniun::Assert_Internal::Assert(#_Expression, __FILE__, __FUNCTION__, __LINE__); \
}

#ifndef NDEBUG
/// Debug Assert exists program if _Expression is false
/// @warning Does not get called if build is set to Dist
///
/// @param _Expression If true, then continue program, otherwise exists out with exit code -1
#define ASSERT(_Expression) FORCE_ASSERT(_Expression)
#else
#define ASSERT(_Expression)
#endif
