#pragma once

#include <cstdlib>

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Compiler.h"

namespace Oniun::Crt
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

    uint64 Format(char* dest, uint64 destMaxLength, const char* format, va_list args);
    uint64 Format(char* dest, uint64 destMaxLength, const char* format, ...);

    FORCE_INLINE void* Allocate(uint64 size)
    {
        void* ptr = std::malloc(size);
        ASSERT(ptr && "Ran out of memory");
        return ptr;
    }

    FORCE_INLINE void Free(void* ptr)
    {
        std::free(ptr);
    }
}
