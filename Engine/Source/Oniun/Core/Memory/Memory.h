#pragma once

#include <new>
#include <type_traits>

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Compiler.h"
#include "Oniun/Core/Memory/Crt.h"

namespace Onu::Memory
{
    constexpr uint64 CalcCapacityGrow(uint64 count, uint64 capacity)
    {
        while (count >= capacity)
        {
            if (capacity < 8)
                capacity = 8;
            else
                capacity = capacity * 2;
        }
        return capacity;
    }
    
    template<typename T>
    FORCE_INLINE void ConstructItem(T* dest)
    {
        if constexpr (!std::is_trivially_constructible_v<T>)
            new(dest) T();
    }

    template<typename T, typename... TArgs>
    FORCE_INLINE void ConstructItemArgs(T* dest, TArgs&&... args)
    {
        new(dest) T(args...);
    }
    
    template<typename T, typename TU>
    FORCE_INLINE void ConstructItem(T* dest, const TU& src)
    {
        static_assert(std::is_constructible_v<T, TU>, "The two types are not constructible");
        if constexpr (!std::is_trivially_constructible_v<T>)
            new(dest) T(src);
        else
            Crt::Copy(dest, &src, sizeof(TU));
    }

    template<typename T>
    FORCE_INLINE void ConstructItems(T* dest, uint64 count)
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
    FORCE_INLINE void ConstructItems(T* dest, const TU* src, uint64 count)
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
    FORCE_INLINE void DestructItem(T* dest)
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
            dest->~T();
    }
        
    template<typename T>
    FORCE_INLINE void DestructItems(T* dest, uint64 count)
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
    FORCE_INLINE void CopyItems(T* dest, const T* src, uint64 count)
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
    FORCE_INLINE void MoveItems(T* dest, const TU* src, uint64 count)
    {
        static_assert(std::is_constructible_v<T, TU>, "The two types are not constructible");
        if constexpr (!std::is_trivially_move_constructible_v<T>)
        {
            for (uint64 i = 0; i < count; ++i)
            {
                new(dest) T(std::move(*src));
                ++dest;
                ++src;
            }
        }
        else
            Crt::Copy(dest, src, count * sizeof(TU));
    }

    template<typename T>
    FORCE_INLINE void SetItems(T* dest, const T& value, uint64 count)
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
}
