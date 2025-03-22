#pragma once

#include "Oniun/Core/Templates/PackedIterator.h"

namespace Oniun
{
    template <typename T>
    class Slice
    {
    public:
        using Type = T;
        using Iterator = PackedIterator<T>;

    public:
        constexpr Slice()
            : m_Data(nullptr), m_Length(0)
        {
        }

        constexpr Slice(T* data, uint64 length)
            : m_Data(data), m_Length(length)
        {
        }

        constexpr Slice(const Iterator& begin, const Iterator& end)
            : m_Data(const_cast<T*>(begin.Ptr())), m_Length(end.Ptr() - begin.Ptr())
        {
        }

        constexpr Slice(const Slice& other) = default;

        constexpr Slice(Slice&& other)
            : m_Data(other.m_Data), m_Length(other.m_Length)
        {
            other.m_Data = nullptr;
            other.m_Length = 0;
            return *this;
        }

    public:
        constexpr Slice& operator=(const Slice& other) = default;

        constexpr Slice& operator=(Slice&& other)
        {
            other.m_Data = nullptr;
            other.m_Length = 0;
            return *this;
        }

        constexpr T& operator[](uint64 index)
        {
            return At(index);
        }

        constexpr const T& operator[](uint64 index) const
        {
            return At(index);
        }

        constexpr bool operator==(const Slice& other)
        {
            return Compare(other);
        }

        constexpr bool operator!=(const Slice& other)
        {
            return !Compare(other);
        }

    public:
        constexpr T* Get() const
        {
            return m_Data;
        }

        constexpr uint64 Length() const
        {
            return m_Length;
        }

        constexpr Iterator begin()
        {
            return Iterator(m_Data);
        }

        constexpr Iterator begin() const
        {
            return Iterator(m_Data);
        }

        constexpr Iterator end()
        {
            return Iterator(m_Data + m_Length);
        }

        constexpr Iterator end() const
        {
            return Iterator(m_Data + m_Length);
        }

        constexpr T& At(uint64 index)
        {
            return m_Data[index];
        }

        constexpr const T& At(uint64 index) const
        {
            return m_Data[index];
        }

    public:
        constexpr bool Contains(const T& value)
        {
            for (uint64 i = 0; i < m_Length; ++i)
            {
                if (value == m_Data[i])
                    return true;
            }
            return false;
        }

        constexpr bool Compare(const Slice& other) const
        {
            if (m_Data == other.m_Data)
                return true;
            if (m_Length != other.m_Length)
                return false;

            for (uint64 i = 0; i < m_Length; ++i)
            {
                if (m_Data[i] != other.m_Data[i])
                    return false;
            }
            return true;
        }

    private:
        T* m_Data;
        uint64 m_Length;
    };

    template <typename T>
    constexpr Slice<T> ToSlice(T* ptr, uint64 length)
    {
        return Slice(ptr, length);
    }

    template <typename T>
    constexpr Slice<T> ToSlice(const PackedIterator<T>& begin, const PackedIterator<T>& end)
    {
        return Slice(begin, end);
    }
}
