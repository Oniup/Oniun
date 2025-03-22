#pragma once

#include "Oniun/Core/Compiler.h"
#include "Oniun/Core/Defines.h"

namespace Oniun
{
    // Primitive types

    using uint8 = unsigned char;
    using uint16 = unsigned short;
    using uint32 = unsigned int;
    using uint64 = unsigned long long;

    using int8 = char;
    using int16 = short;
    using int32 = int;
    using int64 = long long;

    using byte = uint8;

    // Containers forward declarations

    class String;
    class StringView;

    struct HeapAllocation;
    template <uint64 TCapacity>
    struct FixedAllocation;

    template <typename T, typename TAllocationType = HeapAllocation>
    class Array;
    template <typename T, uint64 TCount>
    using FixedArray = Array<T, FixedAllocation<TCount>>;
    template <typename T>
    class LinkList;

    template <typename T>
    struct Hash;
    template <typename TKey, typename TValue, typename THash = Hash<TKey>, typename TAllocationType = HeapAllocation>
    class HashMap;

    template <typename TKey, typename TValue>
    struct Pair;

    template <typename>
    class Function;

    class DateTime;

    class File;

    // Math forward declarations

    struct Vector2;
    struct Vector3;
    struct Vector4;
}
