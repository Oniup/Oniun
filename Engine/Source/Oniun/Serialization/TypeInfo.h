#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/String/String.h"

namespace TypeInfo_Internal
{
    template <typename T>
    constexpr StringView GetWrappedName()
    {
        return TEXT(__PRETTY_FUNCTION__);
    }

    constexpr uint64 GetPrefixLength()
    {
        StringView wrapped(GetWrappedName<void>());
        return wrapped.Find(TEXT("void"));
    }

    constexpr uint64 GetSuffixLength()
    {
        StringView wrapped(GetWrappedName<void>());
        return wrapped.Length() - GetPrefixLength() - 4;
    }

    template <typename T>
    constexpr Slice<Char> GetTypeNameSlice()
    {
        StringView wrapped(GetWrappedName<T>());
        uint64 prefix = GetPrefixLength();
        uint64 length = wrapped.Length() - prefix - GetSuffixLength();
        return ToSlice(wrapped, prefix, length);
    }
}

struct TypeInfo
{
    String Name;
    uint64 Id;
    uint64 Size;
    uint64 Alignment;

    // Serialization delegates

    template <typename T>
    static TypeInfo GetInfo()
    {
        String name(TypeInfo_Internal::GetTypeNameSlice<T>());
        name.Replace(TEXT("class "), TEXT(""));
        name.Replace(TEXT("struct "), TEXT(""));

        return TypeInfo
        {
            .Name = std::move(name),
            .Id = Hash<String>{}.Get(name),
            .Size = sizeof(T),
            .Alignment = alignof(T),
        };
    }

    template <typename T>
    static uint64 GetId()
    {
        String name(TypeInfo_Internal::GetTypeNameSlice<T>());
        name.Replace(TEXT("class "), TEXT(""));
        name.Replace(TEXT("struct "), TEXT(""));
        return Hash<String>{}.Get(name);
    }
};
