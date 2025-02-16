#pragma once

#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/Types/Pair.h"
#include "Oniun/Core/Types/Slice.h"
#include "Oniun/Core/Types/String.h"

namespace Onu
{
    template<typename T>
    struct Hash
    {
        FORCE_INLINE constexpr uint64 Get(const T& src) const
        {
            return Crt::FnvHash(src, sizeof(T));
        }
        
        FORCE_INLINE constexpr uint64 Get(const T* src, uint64 count) const
        {
            return Crt::FnvHash(src, sizeof(T) * count);
        }
    };

    template<>
    struct Hash<String>
    {
        FORCE_INLINE uint64 Get(const String& src) const
        {
            return Crt::FnvHash(src.Data(), sizeof(Char) * src.Length());
        }
    };
    
    template<>
    struct Hash<StringView>
    {
        FORCE_INLINE constexpr uint64 Get(const StringView& src) const
        {
            return Crt::FnvHash(src.Data(), sizeof(Char) * src.Length());
        }
    };

    template<typename T>
    struct Hash<Slice<T>>
    {
        FORCE_INLINE constexpr uint64 Get(const Slice<T>& slice)
        {
            return Hash<T>{}.Get(slice.Get(), slice.Length());
        }
    };
}