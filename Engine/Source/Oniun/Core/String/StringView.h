#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/String/StringUtils.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/Slice.h"

namespace Oniun
{
    class StringView
    {
    public:
        using Iterator = PackedIterator<char>;

    public:
        FORCE_INLINE constexpr StringView()
            : m_Length(0), m_Data(nullptr)
        {
        }

        FORCE_INLINE constexpr StringView(const char* str, uint64 length = NO_POS)
            : m_Length(str == nullptr ? 0 : (length == NO_POS ? StringUtils::Length(str) : length)), m_Data(str)
        {
        }

        FORCE_INLINE constexpr StringView(const Slice<char>& slice)
            : m_Length(slice.Length()), m_Data(slice.Get())
        {
        }

        FORCE_INLINE constexpr StringView(const StringView& str)
            : m_Length(str.m_Length), m_Data(str.m_Data)
        {
        }

        FORCE_INLINE constexpr StringView(StringView&& str)
            : m_Length(str.m_Length), m_Data(str.m_Data)
        {
            str.m_Data = nullptr;
            str.m_Length = 0;
        }

        FORCE_INLINE constexpr StringView(const Iterator& begin, const Iterator& end)
            : m_Length(end - begin), m_Data(begin.Ptr())
        {
        }

        StringView(const String& str);

    public:
        FORCE_INLINE constexpr StringView& operator=(const StringView& str)
        {
            m_Data = str.m_Data;
            m_Length = str.m_Length;
            return *this;
        }

        FORCE_INLINE constexpr StringView& operator=(const char* str)
        {
            m_Data = str;
            if (str)
                m_Length = StringUtils::Length(str);
            else
                m_Length = 0;
            return *this;
        }

        FORCE_INLINE constexpr StringView& operator=(const Slice<char>& slice)
        {
            m_Data = slice.Get();
            if (!slice.IsEmpty())
                m_Length = slice.Length();
            else
                m_Length = 0;
            return *this;
        }

        FORCE_INLINE constexpr StringView& operator=(StringView&& str)
        {
            m_Data = str.m_Data;
            m_Length = str.m_Length;
            str.m_Data = nullptr;
            str.m_Length = 0;
            return *this;
        }

        FORCE_INLINE constexpr const char* operator*() const
        {
            return m_Data;
        }

        FORCE_INLINE constexpr const char& operator[](uint64 index) const
        {
            ASSERT(index <= m_Length);
            return m_Data[index];
        }

        FORCE_INLINE constexpr bool operator==(const StringView& str) const
        {
            return Compare(str) == 0;
        }

        FORCE_INLINE constexpr bool operator!=(const StringView& str) const
        {
            return Compare(str) != 0;
        }

        FORCE_INLINE constexpr bool operator==(const char* str) const
        {
            return Compare(str) == 0;
        }

        FORCE_INLINE constexpr bool operator!=(const char* str) const
        {
            return Compare(str) != 0;
        }

    public:
        FORCE_INLINE constexpr bool IsEmpty() const
        {
            return m_Length == 0;
        }

        FORCE_INLINE constexpr uint64 Length() const
        {
            return m_Length;
        }

        FORCE_INLINE constexpr const char* Data() const
        {
            return m_Data;
        }

        FORCE_INLINE constexpr const char& First() const
        {
            return m_Data[0];
        }

        FORCE_INLINE constexpr const char& Last() const
        {
            return m_Data[m_Length - 1];
        }

        FORCE_INLINE constexpr Iterator begin()
        {
            return Iterator(const_cast<char*>(m_Data));
        }

        FORCE_INLINE constexpr Iterator begin() const
        {
            return Iterator(const_cast<char*>(m_Data));
        }

        FORCE_INLINE constexpr Iterator end()
        {
            return Iterator(const_cast<char*>(m_Data) + m_Length);
        }

        FORCE_INLINE constexpr Iterator end() const
        {
            return Iterator(const_cast<char*>(m_Data) + m_Length);
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

    public:
        constexpr int32 Compare(const StringView& str) const
        {
            if (m_Length != str.m_Length)
            {
                if (m_Length < str.m_Length)
                    return -1;
                return 1;
            }
            return Crt::Compare(m_Data, str.m_Data, m_Length * sizeof(char));
        }

        FORCE_INLINE constexpr void ReCalcLength()
        {
            m_Length = StringUtils::Length(m_Data);
        }

        constexpr uint64 Find(const StringView& find, StringSearch opts = StringSearch::CaseSensitive,
                              uint64 offset = 0) const
        {
            if (opts == StringSearch::CaseSensitive)
                return StringUtils::Find(m_Data, m_Length, find.m_Data, find.m_Length, offset);
            return StringUtils::FindIgnoreCase(m_Data, m_Length, find.m_Data, find.m_Length, offset);
        }

        constexpr uint64 FindLast(const StringView& find, StringSearch opts = StringSearch::CaseSensitive,
                                  uint64 offset = 0) const
        {
            if (opts == StringSearch::CaseSensitive)
                return StringUtils::FindLast(m_Data, m_Length, find.m_Data, find.m_Length, offset);
            return StringUtils::FindLastIgnoreCase(m_Data, m_Length, find.m_Data, find.m_Length, offset);
        }

        bool FindAll(const StringView& find, Array<uint64>& indices) const
        {
            if (find.Length() > m_Length)
                return false;

            for (uint64 i = 0; i < m_Length - find.Length() + 1; ++i)
            {
                bool found = true;
                for (uint64 j = 0; j < find.Length(); ++j)
                {
                    if (m_Data[i + j] != find[j])
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                    indices.Add(i);
            }
            return indices.Count() > 0;
        }

        constexpr uint64 Find(char find, uint64 offset = 0) const
        {
            return StringUtils::Find(m_Data, m_Length, find, offset);
        }

        constexpr uint64 FindLast(char find, uint64 offset = 0) const
        {
            return StringUtils::FindLast(m_Data, m_Length, find, offset);
        }

        bool FindAll(char find, Array<uint64>& indices) const
        {
            for (uint64 i = 0; i < m_Length; ++i)
            {
                if (m_Data[i] == find)
                    indices.Add(i);
            }
            return indices.Count() > 0;
        }

        constexpr bool BeginsWith(const StringView& str) const
        {
            return StringUtils::BeginsWith(m_Data, m_Length, str.m_Data, str.m_Length);
        }

        constexpr bool EndsWith(const StringView& str) const
        {
            return StringUtils::EndsWith(m_Data, m_Length, str.m_Data, str.m_Length);
        }

    private:
        uint64 m_Length;
        const char* m_Data;
    };

    constexpr Slice<char> ToSlice(const StringView& string)
    {
        return Slice(const_cast<char*>(string.Data()), string.Length());
    }

    constexpr Slice<char> ToSlice(const StringView& string, uint64 index, uint64 length)
    {
        return Slice(const_cast<char*>(string.Data()) + index, length);
    }
}
