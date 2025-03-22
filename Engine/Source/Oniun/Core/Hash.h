#pragma once

#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Pair.h"
#include "Oniun/Core/Templates/Slice.h"

namespace Hash_Internal
{
    /// @brief Fowler-Noll-Vo hash function.
    ///        Source from: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    /// @param src  Packed data to be C style cast to uint8 bytes array inorder to calculate the hash value
    /// @param size Size of the packed data
    /// @return Unique uint64 hash value
    FORCE_INLINE constexpr uint64 FnvHash(const char* src, uint64 size)
    {
        constexpr uint64 prime = 0x00000100000001b3;
        uint64 hash = 0xcbf29ce484222325;
        for (uint64 i = 0; i < size; i++)
        {
            hash *= prime;
            hash ^= src[i];
        }
        return hash;
    }
}

template <typename T>
struct Hash
{
    FORCE_INLINE constexpr uint64 Get(const T& src) const = delete;
};

template <>
struct Hash<const char*>
{
    FORCE_INLINE constexpr uint64 Get(const char* src) const
    {
        return Hash_Internal::FnvHash(src, StringUtils::Length(src));
    }
};

template <>
struct Hash<String>
{
    FORCE_INLINE uint64 Get(const String& src) const
    {
        return Hash_Internal::FnvHash(src.Data(), src.Length());
    }
};

template <>
struct Hash<StringView>
{
    FORCE_INLINE constexpr uint64 Get(const StringView& src) const
    {
        return Hash_Internal::FnvHash(src.Data(), src.Length());
    }
};

template <>
struct Hash<Slice<char>>
{
    FORCE_INLINE constexpr uint64 Get(const Slice<char>& slice) const
    {
        return Hash_Internal::FnvHash(slice.Get(), slice.Length());
    }
};

template <>
struct Hash<uint64>
{
    FORCE_INLINE constexpr uint64 Get(uint64 val) const
    {
        // https://stackoverflow.com/a/12996028
        val = (val ^ (val >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        val = (val ^ (val >> 27)) * UINT64_C(0x94d049bb133111eb);
        val = val ^ (val >> 31);
        return val;
    }
};

template <>
struct Hash<uint32>
{
    FORCE_INLINE constexpr uint64 Get(uint32 val) const
    {
        // https://stackoverflow.com/a/12996028
        val = ((val >> 16) ^ val) * 0x45d9f3b;
        val = ((val >> 16) ^ val) * 0x45d9f3b;
        val = (val >> 16) ^ val;
        return val;
    }
};
