#pragma once

#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Pair.h"
#include "Oniun/Core/Templates/Slice.h"

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
    struct Hash<CharString>
    {
        FORCE_INLINE uint64 Get(const CharString& src) const
        {
            return Crt::FnvHash(src.Data(), sizeof(char) * src.Length());
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

    template<>
    struct Hash<CharStringView>
    {
        FORCE_INLINE constexpr uint64 Get(const CharStringView& src) const
        {
            return Crt::FnvHash(src.Data(), sizeof(char) * src.Length());
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
