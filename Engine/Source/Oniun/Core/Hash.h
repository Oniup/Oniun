#pragma once

#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Pair.h"
#include "Oniun/Core/Templates/Slice.h"

template <typename T>
struct Hash
{
    FORCE_INLINE constexpr uint64 Get(const T& src) const
    {
        if constexpr (std::is_pointer_v<T>)
            return Crt::FnvHash(src, sizeof(T));
        else
            return Crt::FnvHash(&src, sizeof(T));
    }

    FORCE_INLINE constexpr uint64 Get(const T* src, uint64 count) const
    {
        return Crt::FnvHash(src, sizeof(T) * count);
    }
};

template <>
struct Hash<String>
{
    FORCE_INLINE uint64 Get(const String& src) const
    {
        return Crt::FnvHash(src.Data(), src.Length());
    }
};

template <>
struct Hash<StringView>
{
    FORCE_INLINE constexpr uint64 Get(const StringView& src) const
    {
        return Crt::FnvHash(src.Data(), src.Length());
    }
};

template <typename T>
struct Hash<Slice<T>>
{
    FORCE_INLINE constexpr uint64 Get(const Slice<T>& slice) const
    {
        return Hash<T>{}.Get(slice.Get(), slice.Length());
    }
};
