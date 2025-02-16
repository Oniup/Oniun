#pragma once

#include "Oniun/Core/Compiler.h"
#include "Oniun/Core/Defines.h"

// Primitive types

using Char = wchar_t;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;

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

namespace Onu
{
    // Containers forward declarations
    
    class String;
    class StringView;
    
    struct HeapAllocation;
    template<uint64 TCapacity>
    struct FixedAllocation;
    
    template<typename T, typename TAllocationType>
    class Array;
    template<typename T, uint64 TCount>
    using FixedArray = Array<T, FixedAllocation<TCount>>;
    template<typename TKey, typename TValue, typename THash, typename TAllocationType>
    class HashMap;

    template<typename... TParams>
    class Delegate;

    template<typename TKey, typename TValue>
    class Pair;
}
