#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/GlobalVars.h"
#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/String/StringUtils.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/Slice.h"

template <typename TChar>
class IStringView
{
public:
    using CharType = TChar;
    using Iterator = PackedIterator<TChar>;

protected:
    uint64 m_Length;
    const TChar* m_Data;

public:
    FORCE_INLINE constexpr IStringView()
        : m_Length(0), m_Data(nullptr)
    {
    }

    FORCE_INLINE constexpr IStringView(const TChar* str, uint64 length = GlobalVars::NoPos)
        : m_Length(length == GlobalVars::NoPos ? StringUtils::Length(str) : length), m_Data(str)
    {
    }

    FORCE_INLINE constexpr IStringView(const Slice<TChar>& slice)
        : m_Length(slice.Length()), m_Data(slice.Get())
    {
    }

    FORCE_INLINE constexpr IStringView& operator=(const IStringView& str)
    {
        m_Data = str.m_Data;
        m_Length = str.m_Length;
        return *this;
    }

    FORCE_INLINE constexpr IStringView& operator=(const TChar* str)
    {
        m_Data = str;
        m_Length = StringUtils::Length(str);
        return *this;
    }

    FORCE_INLINE constexpr IStringView& operator=(const Slice<TChar>& slice)
    {
        m_Data = slice.Get();
        m_Length = slice.Length();
        return *this;
    }

    FORCE_INLINE constexpr IStringView& operator=(IStringView&& str)
    {
        m_Data = str.m_Data;
        m_Length = str.m_Length;
        str.m_Data = nullptr;
        str.m_Length = 0;
        return *this;
    }

    FORCE_INLINE constexpr const TChar* operator*() const
    {
        return m_Data;
    }

    FORCE_INLINE constexpr const TChar& operator[](uint64 index) const
    {
        ASSERT(index <= m_Length);
        return m_Data[index];
    }

    FORCE_INLINE constexpr bool operator==(const IStringView& str) const
    {
        return Compare(str) == 0;
    }

    FORCE_INLINE constexpr bool operator!=(const IStringView& str) const
    {
        return Compare(str) != 0;
    }

    FORCE_INLINE constexpr bool operator==(const TChar* str) const
    {
        return Compare(str) == 0;
    }

    FORCE_INLINE constexpr bool operator!=(const TChar* str) const
    {
        return Compare(str) != 0;
    }

    FORCE_INLINE constexpr bool IsEmpty() const
    {
        return m_Data == nullptr;
    }

    FORCE_INLINE constexpr uint64 Length() const
    {
        return m_Length;
    }

    FORCE_INLINE constexpr const TChar* Data() const
    {
        return m_Data;
    }

    FORCE_INLINE constexpr TChar& First()
    {
        return m_Data[0];
    }

    FORCE_INLINE constexpr const TChar& First() const
    {
        return m_Data[0];
    }

    FORCE_INLINE constexpr TChar& Last()
    {
        return m_Data[m_Length - 1];
    }

    FORCE_INLINE constexpr const TChar& Last() const
    {
        return m_Data[m_Length - 1];
    }

    FORCE_INLINE constexpr Iterator begin()
    {
        return Iterator(const_cast<TChar*>(m_Data));
    }

    FORCE_INLINE constexpr Iterator begin() const
    {
        return Iterator(const_cast<TChar*>(m_Data));
    }

    FORCE_INLINE constexpr Iterator end()
    {
        return Iterator(const_cast<TChar*>(m_Data) + m_Length);
    }

    FORCE_INLINE constexpr Iterator end() const
    {
        return Iterator(const_cast<TChar*>(m_Data) + m_Length);
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

    constexpr int32 Compare(const IStringView& str) const
    {
        if (m_Length != str.m_Length)
        {
            if (m_Length < str.m_Length)
                return -1;
            return 1;
        }
        return Crt::Compare(m_Data, str.m_Data, m_Length * sizeof(TChar));
    }

    FORCE_INLINE constexpr void ReCalcLength()
    {
        m_Length = StringUtils::Length(m_Data);
    }
};

class StringView : public IStringView<Char>
{
    friend String;

public:
    FORCE_INLINE constexpr StringView()
    {
    }

    FORCE_INLINE constexpr StringView(const Char* str)
        : IStringView(str, StringUtils::Length(str))
    {
    }

    FORCE_INLINE constexpr StringView(const Char* str, uint64 length)
        : IStringView(str, length)
    {
    }

    FORCE_INLINE constexpr StringView(const Slice<Char>& slice)
        : IStringView(slice)
    {
    }

    FORCE_INLINE constexpr StringView(const StringView& str)
        : IStringView(str.m_Data, str.m_Length)
    {
    }

    FORCE_INLINE constexpr StringView(StringView&& str)
        : IStringView(str.m_Data, str.m_Length)
    {
        str.m_Data = nullptr;
        str.m_Length = 0;
    }

    StringView(const String& str);

    String operator+(Char ch) const;
    String operator+(const StringView& str) const;
    String operator/(Char ch) const;
    String operator/(const String& str) const;

    constexpr uint64 Find(const StringView& find, StringSearch opts = StringSearch::CaseSensitive, uint64 offset = 0) const
    {
        if (opts == StringSearch::CaseSensitive)
            return StringUtils::Find(m_Data, m_Length, find.m_Data, find.m_Length, offset);
        return StringUtils::FindIgnoreCase(m_Data, m_Length, find.m_Data, find.m_Length, offset);
    }

    constexpr uint64 FindLast(const StringView& find, StringSearch opts = StringSearch::CaseSensitive, uint64 offset = 0) const
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

    constexpr uint64 Find(Char find, uint64 offset = 0) const
    {
        return StringUtils::Find(m_Data, m_Length, find, offset);
    }

    constexpr uint64 FindLast(Char find, uint64 offset = 0) const
    {
        return StringUtils::FindLast(m_Data, m_Length, find, offset);
    }

    bool FindAll(Char find, Array<uint64>& indices) const
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
};

class CharStringView : public IStringView<char>
{
    friend CharString;

public:
    FORCE_INLINE constexpr CharStringView()
    {
    }

    FORCE_INLINE constexpr CharStringView(const char* str)
        : IStringView(str, StringUtils::Length(str))
    {
    }

    FORCE_INLINE constexpr CharStringView(const char* str, uint64 length)
        : IStringView(str, length)
    {
    }

    FORCE_INLINE constexpr CharStringView(const Slice<char>& slice)
        : IStringView(slice)
    {
    }

    FORCE_INLINE constexpr CharStringView(const CharStringView& str)
        : IStringView(str.m_Data, str.m_Length)
    {
    }

    FORCE_INLINE constexpr CharStringView(CharStringView&& str)
        : IStringView(str.m_Data, str.m_Length)
    {
        str.m_Data = nullptr;
        str.m_Length = 0;
    }

    CharStringView(const CharString& str);

    CharString operator+(char ch) const;
    CharString operator+(const CharStringView& str) const;
    CharString operator/(char ch) const;
    CharString operator/(const CharStringView& str) const;

    constexpr uint64 Find(const CharStringView& find, StringSearch opts = StringSearch::CaseSensitive, uint64 offset = 0) const
    {
        if (opts == StringSearch::CaseSensitive)
            return StringUtils::Find(m_Data, m_Length, find.m_Data, find.m_Length, offset);
        return StringUtils::FindIgnoreCase(m_Data, m_Length, find.m_Data, find.m_Length, offset);
    }

    constexpr uint64 FindLast(const CharStringView& find, StringSearch opts = StringSearch::CaseSensitive, uint64 offset = 0) const
    {
        if (opts == StringSearch::CaseSensitive)
            return StringUtils::FindLast(m_Data, m_Length, find.m_Data, find.m_Length, offset);
        return StringUtils::FindLastIgnoreCase(m_Data, m_Length, find.m_Data, find.m_Length, offset);
    }

    bool FindAll(const CharStringView& find, Array<uint64>& indices) const
    {
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

    constexpr bool BeginsWith(const CharStringView& str) const
    {
        return StringUtils::BeginsWith(m_Data, m_Length, str.m_Data, str.m_Length);
    }

    constexpr bool EndsWith(const CharStringView& str) const
    {
        return StringUtils::EndsWith(m_Data, m_Length, str.m_Data, str.m_Length);
    }
};

constexpr Slice<Char> ToSlice(const StringView& string)
{
    return Slice(const_cast<Char*>(string.Data()), string.Length());
}

constexpr Slice<Char> ToSlice(const StringView& string, uint64 index, uint64 length)
{
    return Slice(const_cast<Char*>(string.Data()) + index, length);
}

constexpr Slice<char> ToSlice(const CharStringView& string)
{
    return Slice(const_cast<char*>(string.Data()), string.Length());
}

constexpr Slice<char> ToSlice(const CharStringView& string, uint64 index, uint64 length)
{
    return Slice(const_cast<char*>(string.Data()) + index, length);
}
