#pragma once

#include <initializer_list>

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/Templates/PackedIterator.h"
#include "Oniun/Core/Templates/Slice.h"

namespace Oniun
{
    /// Dynamic or fixed array depending on the allocation type provided
    ///
    /// @tparam T               List of types to be contained and manipulated in the array
    /// @tparam TAllocationType Allocation type, can either of Oniun's allocators HeapAllocation, FixedAllocation or a
    ///                         custom one with the required structure and functions
    template <typename T, typename TAllocationType>
    class Array
    {
    public:
        using Type = T;
        using Allocator = typename TAllocationType::template Data<T>;
        using Iterator = PackedIterator<T>;

    public:
        constexpr Array()
            : m_Count(0)
        {
        }

        constexpr Array(uint64 capacity)
            : m_Count(0)
        {
            m_Data.Allocate(capacity);
        }

        constexpr Array(uint64 count, uint64 capacity)
            : m_Count(count)
        {
            DEBUG_ASSERT(count < capacity);
            m_Data.Allocate(capacity);
            Memory::ConstructItems(m_Data.Ptr(), m_Count);
        }

        constexpr Array(const T* data, uint64 count)
            : m_Count(count)
        {
            m_Data.Allocate(count);
            Memory::ConstructItems(m_Data.Ptr(), data, m_Count);
        }

        constexpr Array(const Array& other)
            : m_Count(other.m_Count)
        {
            m_Data.Allocate(other.Capacity());
            Memory::ConstructItems(m_Data.Ptr(), other.m_Data.Ptr(), m_Count);
        }

        constexpr Array(Array&& other)
            : m_Count(other.m_Count)
        {
            m_Data.Move(Memory::Move(other.m_Data));
            other.m_Count = 0;
        }

        constexpr Array(Iterator& begin, Iterator& end)
            : m_Count(end.Ptr() - begin.Ptr())
        {
            if (m_Count > 0)
            {
                m_Data.Allocate(m_Count);
                uint64 count = end - begin;
                if (count != 0)
                {
                    Memory::ConstructItems(m_Data.Ptr(), begin.Ptr(), count);
                }
            }
        }

        constexpr Array(const std::initializer_list<T>& initList)
            : m_Count(initList.size())
        {
            m_Data.Allocate(m_Count);
            Memory::ConstructItems(m_Data.Ptr(), &*initList.begin(), m_Count);
        }

        constexpr ~Array()
        {
            Clear();
        }

    public:
        constexpr bool operator==(const Array& array) const
        {
            if (m_Count != array.m_Count)
                return false;
            for (uint64 i = 0; i < m_Count; ++i)
            {
                if (m_Data[i] != array.m_Data[i])
                    return false;
            }
            return true;
        }

        FORCE_INLINE constexpr bool operator!=(const Array& array) const
        {
            return !(*this == array);
        }

        FORCE_INLINE constexpr Array& operator=(const Array& array)
        {
            Clear();
            Resize(array.m_Count);
            Memory::ConstructItems(m_Data.Ptr(), array.m_Data.Ptr(), array.m_Count);
            return *this;
        }

        FORCE_INLINE constexpr Array& operator=(
            Array&& array)
        {
            Free();
            m_Data.Move(array.m_Data);
            m_Count = array.m_Count;
            array.m_Count = 0;
            return *this;
        }

        FORCE_INLINE constexpr T& operator[](uint64 index)
        {
            return Get(index);
        }

        FORCE_INLINE constexpr const T& operator[](uint64 index) const
        {
            return Get(index);
        }

    public:
        FORCE_INLINE constexpr uint64 Count() const
        {
            return m_Count;
        }

        FORCE_INLINE constexpr uint64 Capacity() const
        {
            return m_Data.Capacity();
        }

        FORCE_INLINE constexpr Allocator& GetAllocator()
        {
            return m_Data;
        }

        FORCE_INLINE constexpr const Allocator& GetAllocator() const
        {
            return m_Data;
        }

        FORCE_INLINE constexpr T* Data()
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE constexpr const T* Data() const
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE constexpr T& Front()
        {
            return m_Data[0];
        }

        FORCE_INLINE constexpr const T& Front() const
        {
            return m_Data[0];
        }

        FORCE_INLINE constexpr T& Back()
        {
            return m_Count > 0 ? m_Data[m_Count - 1] : Front();
        }

        FORCE_INLINE constexpr const T& Back() const
        {
            return m_Count > 0 ? m_Data[m_Count - 1] : Front();
        }

        FORCE_INLINE constexpr T& Get(uint64 index)
        {
            return m_Data[index];
        }

        FORCE_INLINE constexpr const T& Get(uint64 index) const
        {
            return m_Data[index];
        }

        FORCE_INLINE constexpr Iterator begin()
        {
            return Iterator(m_Data.Ptr());
        }

        FORCE_INLINE constexpr Iterator begin() const
        {
            return Iterator(const_cast<T*>(m_Data.Ptr()));
        }

        FORCE_INLINE constexpr Iterator end()
        {
            return Iterator(m_Data.Ptr() + m_Count);
        }

        FORCE_INLINE constexpr Iterator end() const
        {
            return Iterator(const_cast<T*>(m_Data.Ptr()) + m_Count);
        }

        FORCE_INLINE constexpr Iterator Begin()
        {
            return begin();
        }

        FORCE_INLINE constexpr Iterator Begin() const
        {
            return begin();
        }

        FORCE_INLINE constexpr Iterator End()
        {
            return end();
        }

        FORCE_INLINE constexpr Iterator End() const
        {
            return end();
        }

        FORCE_INLINE constexpr T& First()
        {
            return m_Data[0];
        }

        FORCE_INLINE constexpr const T& First() const
        {
            return m_Data[0];
        }

        FORCE_INLINE constexpr T& Last()
        {
            return m_Data[m_Count - 1];
        }


        FORCE_INLINE constexpr const T& Last() const
        {
            return m_Data[m_Count - 1];
        }

        FORCE_INLINE constexpr bool IsEmpty() const
        {
            return m_Count == 0;
        }

    public:
        constexpr void Clear()
        {
            if (m_Data.Ptr())
                Memory::DestructItems(m_Data.Ptr(), m_Count);
            m_Count = 0;
        }

        constexpr void Free()
        {
            if (m_Data.Ptr())
            {
                if (m_Data.Ptr())
                    Memory::DestructItems(m_Data.Ptr(), m_Count);
                m_Data.Destroy();
            }
            m_Count = 0;
        }

        constexpr bool Compare(const Array& other)
        {
            if (m_Data.Ptr() == other.m_Data.Ptr())
                return true;
            if (m_Count != other.m_Count)
                return false;

            for (uint64 i = 0; i < m_Count; ++i)
            {
                if (m_Data[i] != other.m_Data[i])
                    return false;
            }
            return false;
        }

        FORCE_INLINE constexpr bool Resize(uint64 count)
        {
            if (count > Capacity())
            {
                if constexpr (std::is_same_v<TAllocationType, HeapAllocation>)
                {
                    Reserve(Memory::CalcCapacityGrow(count, Capacity()));
                    // Memory::ConstructItems(m_Data.Ptr() + m_Count, count - m_Count);
                }
                else
                    return false;
            }
            m_Count = count;
            return true;
        }

        FORCE_INLINE constexpr void Reserve(uint64 newCapacity, bool preserveContents = true)
        {
            if constexpr (std::is_same_v<TAllocationType, HeapAllocation>)
            {
                uint64 count = preserveContents ? (m_Count < newCapacity ? m_Count : newCapacity) : 0;
                m_Data.Relocate(newCapacity, m_Count, count);
            }
        }

        constexpr void Add(const T& value)
        {
            if (Resize(m_Count + 1))
                Memory::ConstructItem(&m_Data[m_Count - 1], value);
        }

        constexpr void Add(T&& value)
        {
            if (Resize(m_Count + 1))
                Memory::ConstructItem(&m_Data[m_Count - 1], value);
        }

        constexpr void Insert(const T& value, uint64 index)
        {
            DEBUG_ASSERT(index <= m_Count);
            if (Resize(m_Count + 1))
            {
                Crt::Move(m_Data.Ptr() + index + 1, m_Data.Ptr() + index, sizeof(T) * (m_Count - index));
                Memory::ConstructItem(m_Data.Ptr() + index, value);
            }
        }

        constexpr void Insert(T&& value, uint64 index)
        {
            DEBUG_ASSERT(index <= m_Count);
            if (Resize(m_Count + 1))
            {
                Crt::Move(m_Data.Ptr() + index + 1, m_Data.Ptr() + index, sizeof(T) * (m_Count - index));
                Memory::MoveItems(m_Data.Ptr() + index, &value, 1);
            }
        }

        constexpr void Remove(const T& value)
        {
            for (uint64 i = 0; i < m_Count; ++i)
            {
                if (m_Data[i] == value)
                {
                    RemoveAt(i);
                    return;
                }
            }
        }

        constexpr void RemoveAt(uint64 index)
        {
            DEBUG_ASSERT(index <= m_Count);
            Memory::DestructItem(&m_Data[index]);
            Crt::Move(m_Data.Ptr() + index, m_Data.Ptr() + index + 1, sizeof(T) * (m_Count - index - 1));
            Resize(m_Count - 1);
        }

        constexpr void RemoveAt(Iterator iterator)
        {
            DEBUG_ASSERT(iterator < end());
            RemoveAt(end() - iterator);
        }

        constexpr void RemoveLast()
        {
            DEBUG_ASSERT(m_Count > 0);
            Memory::DestructItem(&Back());
            Resize(m_Count - 1);
        }

        constexpr T Pop()
        {
            DEBUG_ASSERT(m_Count > 0);
            T item(Memory::Move(Back()));
            Resize(m_Count - 1);
            return item;
        }

        template <typename TComparableType>
        constexpr uint64 Find(const TComparableType& item) const
        {
            for (uint64 i = 0; i < m_Count; ++i)
            {
                if (m_Data[i] == item)
                    return i;
            }
            return NO_POS;
        }

        template <typename TComparableType>
        constexpr uint64 FindLast(const TComparableType& item) const
        {
            uint64 last = NO_POS;
            for (uint64 i = 0; i < m_Count; ++i)
            {
                if (m_Data[i] == item)
                    last = i;
            }
            return last;
        }

        constexpr std::enable_if<std::is_same_v<TAllocationType, HeapAllocation>> Swap(Array& other)
        {
            Memory::Swap(m_Data, other.m_Data);

            uint64 countTemp = m_Count;
            m_Count = other.m_Count;
            other.m_Count = countTemp;
            return;
        }

    private:
        uint64 m_Count;
        Allocator m_Data;
    };

    template <typename T, uint64 TCount>
    using FixedArray = Array<T, FixedAllocation<TCount>>;

    template <typename T, typename TAllocationType>
    constexpr Slice<T> ToSlice(const Array<T, TAllocationType>& array)
    {
        return Slice(const_cast<T*>(array.Data()), array.Count());
    }

    template <typename T, typename TAllocationType>
    constexpr Slice<T> ToSlice(const Array<T, TAllocationType>& array, uint64 index, uint64 count)
    {
        DEBUG_ASSERT(index + count <= array.Count());
        return Slice(array.Data() + index, count);
    }
}
