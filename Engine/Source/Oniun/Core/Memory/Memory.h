#pragma once

#include <new>

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Compiler.h"
#include "Oniun/Core/Memory/Crt.h"

namespace Memory
{
    template<typename T>
    FORCE_INLINE constexpr std::remove_reference_t<T>&& Move(T&& item)
    {
        return static_cast<std::remove_reference_t<T>&&>(item);
    }

    template <typename T>
    FORCE_INLINE constexpr T&& Forward(std::remove_reference_t<T>& arg)
    {
        return static_cast<T&&>(arg);
    }

    template <typename T>
    FORCE_INLINE constexpr std::remove_reference_t<T>&& Forward(std::remove_reference_t<T>&& arg)
    {
        static_assert(!std::is_lvalue_reference_v<T>, "bad forward call");
        return static_cast<T&&>(arg);
    }

    template<typename T>
    FORCE_INLINE constexpr void Swap(T& val0, T& val1)
    {
        T temp = Move(val0);
        val0 = Move(val1);
        val1 = Move(temp);
    }

    FORCE_INLINE constexpr uint64 CalcCapacityGrow(uint64 count, uint64 capacity)
    {
        if (capacity < count)
            capacity = count;
        if (capacity < 8)
            capacity = 8;
        else
        {
            capacity--;
            capacity |= capacity >> 1;
            capacity |= capacity >> 2;
            capacity |= capacity >> 4;
            capacity |= capacity >> 8;
            capacity |= capacity >> 16;
            capacity = capacity + 1 * 2;
        }
        return capacity;
    }

    template<typename T>
    FORCE_INLINE constexpr void ConstructItem(T* dest)
    {
        if constexpr (!std::is_trivially_constructible_v<T>)
            new(dest) T();
    }

    template<typename T, typename... TArgs>
    FORCE_INLINE constexpr void ConstructItem(T* dest, TArgs&&... args)
    {
        new(dest) T(Forward<TArgs>(args)...);
    }

    template<typename T>
    FORCE_INLINE constexpr void ConstructItems(T* dest, uint64 count)
    {
        if constexpr (!std::is_trivially_constructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                new(dest) T();
                ++static_cast<T*&>(dest);
            }
        }
    }

    template<typename T, typename TU>
    FORCE_INLINE constexpr void ConstructItems(T* dest, const TU* src, uint64 count)
    {
        static_assert(std::is_constructible_v<T, TU>, "The two types are not constructible");
        if constexpr (!std::is_trivially_copy_constructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                new(dest) T(*src);
                ++dest;
                ++src;
            }
        }
        else
            Crt::Copy(dest, src, count * sizeof(TU));
    }

    template<typename T>
    FORCE_INLINE constexpr void DestructItem(T* dest)
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
            dest->~T();
    }

    template<typename T>
    FORCE_INLINE constexpr void DestructItems(T* dest, uint64 count)
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                dest->~T();
                ++dest;
            }
        }
    }

    template<typename T>
    FORCE_INLINE constexpr void CopyItems(T* dest, const T* src, uint64 count)
    {
        if constexpr (!std::is_trivially_copy_constructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                *dest = *src;
                ++dest;
                ++src;
            }
        }
        else
            Crt::Copy(dest, src, count * sizeof(T));
    }

    template<typename T, typename TU>
    FORCE_INLINE constexpr void MoveItems(T* dest, TU* src, uint64 count)
    {
        static_assert(std::is_constructible_v<T, TU>, "The two types are not constructible");
        if constexpr (!std::is_trivially_move_constructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                new(dest) T(Move(*src));
                ++dest;
                ++src;
            }
        }
        else
            Crt::Copy(dest, src, count * sizeof(TU));
    }

    template<typename T>
    FORCE_INLINE constexpr void SetItems(T* dest, const T& value, uint64 count)
    {
        if constexpr (!std::is_trivially_copy_constructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                new(dest) T(value);
                ++dest;
            }
        }
        else
            Crt::Copy(dest, value, count * sizeof(T));
    }

    template<typename T, typename... TArgs>
    FORCE_INLINE constexpr T* New(TArgs&&... args)
    {
        T* ptr = static_cast<T*>(Crt::Allocate(sizeof(T)));
        new(ptr) T(Forward<TArgs>(args)...);
        return ptr;
    }

    template<typename T>
    FORCE_INLINE constexpr void Delete(T* ptr)
    {
        DestructItem(ptr);
        Crt::Free(ptr);
    }
}
