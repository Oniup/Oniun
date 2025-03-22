#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/String/String.h"

namespace Oniun
{
    namespace TypeInfo_Internal
    {
        template <typename T>
        struct RemoveAllPointers
        {
            using Type = T;
        };

        template <typename T>
        struct RemoveAllPointers<T*>
        {
            using Type = typename RemoveAllPointers<T>::Type;
        };

        template <typename T>
        struct TypeStripped
        {
            using TypeRemovedExtents = std::remove_all_extents_t<T>;
            using TypeRemovedRefs = std::remove_reference_t<TypeRemovedExtents>;
            using TypeRemovedPointers = typename RemoveAllPointers<TypeRemovedRefs>::Type;
            using Type = std::remove_cvref_t<TypeRemovedPointers>;
        };

        template <typename T>
        constexpr StringView GetWrappedName()
        {
            return __PRETTY_FUNCTION__;
        }

        constexpr uint64 GetPrefixLength()
        {
            StringView wrapped(GetWrappedName<void>());
            return wrapped.Find("void");
        }

        constexpr uint64 GetSuffixLength()
        {
            StringView wrapped(GetWrappedName<void>());
            return wrapped.Length() - GetPrefixLength() - 4;
        }

        template <typename T>
        constexpr Slice<char> GetTypeNameSlice()
        {
            StringView wrapped(GetWrappedName<T>());
            uint64 prefix = GetPrefixLength();
            uint64 length = wrapped.Length() - prefix - GetSuffixLength();
            return ToSlice(wrapped, prefix, length);
        }
    }

    struct TypeInfo
    {
        uint64 Id;
        uint64 FastId;

        String Name;
        uint64 Size;
        uint64 Alignment;

        // Serialization delegates

        template <typename T>
        static TypeInfo GetInfo()
        {
            using Type = typename TypeInfo_Internal::TypeStripped<T>::Type;

            String name(TypeInfo_Internal::GetTypeNameSlice<Type>());
            name.Replace("class ", "");
            name.Replace("struct ", "");

            return TypeInfo
            {
                .Id = Hash<String>{}.Get(name),
                .FastId = TypeInfo::GetFastId<Type>(),
                .Name = Memory::Move(name),
                .Size = sizeof(Type),
                .Alignment = alignof(Type),
            };
        }

        template <typename T>
        static uint64 GetId()
        {
            String name(TypeInfo_Internal::GetTypeNameSlice<typename TypeInfo_Internal::TypeStripped<T>::Type>());
            name.Replace("class ", "");
            name.Replace("struct ", "");
            return Hash<String>{}.Get(name);
        }

        template <typename T>
        static constexpr uint64 GetFastId()
        {
            Slice name(TypeInfo_Internal::GetTypeNameSlice<typename TypeInfo_Internal::TypeStripped<T>::Type>());
            return Hash<Slice<char>>{}.Get(name);
        }
    };
}
