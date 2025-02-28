#pragma once

#include <cstdlib>

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Compiler.h"

namespace Crt
{
    FORCE_INLINE constexpr void Copy(void* dest, const void* src, uint64 size)
    {
        uint8* destPtr = static_cast<uint8*>(dest);
        const uint8* srcPtr = static_cast<const uint8*>(src);
        for (uint64 i = 0; i < size; ++i)
        {
            destPtr[i] = srcPtr[i];
        }
    }

    FORCE_INLINE constexpr void Move(void* dest, const void* src, uint64 size)
    {
        uint8* destPtr = static_cast<uint8*>(dest);
        const uint8* srcPtr = static_cast<const uint8*>(src);
        if (dest < src)
        {
            for (uint64 i = 0; i < size; ++i)
                destPtr[i] = srcPtr[i];
        }
        else if (dest > src)
        {
            for (uint64 i = size; i > 0; --i)
                destPtr[i - 1] = srcPtr[i - 1];
        }
    }

    FORCE_INLINE constexpr void Clear(void* dest, uint64 size)
    {
        uint8* destPtr = static_cast<uint8*>(dest);
        for (uint64 i = 0; i < size; ++i)
            destPtr[i] = 0;
    }

    FORCE_INLINE constexpr int32 Compare(const void* dest, const void* src, uint64 size)
    {
        const uint8* destPtr = static_cast<const uint8*>(dest);
        const uint8* srcPtr = static_cast<const uint8*>(src);
        for (uint64 i = 0; i < size; ++i)
        {
            if (destPtr[i] < srcPtr[i])
                return -1;
            if (destPtr[i] > srcPtr[i])
                return 1;
        }
        return 0;
    }

    FORCE_INLINE void* Allocate(uint64 size)
    {
        return std::malloc((size_t)size);
    }

    FORCE_INLINE void Free(void* ptr)
    {
        std::free(ptr);
    }

    /// @brief Fowler-Noll-Vo hash function.
    ///        Source from: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    /// @param src  Packed data to be C style cast to uint8 bytes array inorder to calculate the hash value
    /// @param size Size of the packed data
    /// @return     Unique uint64 hash value
    FORCE_INLINE constexpr uint64 FnvHash(const void* src, uint64 size)
    {
        constexpr uint64 prime = 0x00000100000001b3;
        const uint8* bytes = (const uint8*)src;
        uint64 hash = 0xcbf29ce484222325;
        for (uint64 i = 0; i < size; i++)
        {
            hash *= prime;
            hash ^= bytes[i];
        }
        return hash;
    }
}
