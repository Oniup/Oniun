#include "Oniun/Core/String/String.h"

#include <cwchar>

#include "Oniun/Core/String/StringUtils.h"

namespace Onu
{
    const String String::Empty = String();
    const CharString CharString::Empty = CharString();

    bool String::Parse(const StringView& str, int64* result)
    {
        bool isNegative = false;
        int64 sum = 0;
        for (Char ch : str)
        {
            if (ch == L'-')
            {
                isNegative = true;
                continue;
            }

            uint64 num = ch - static_cast<uint64>('0');
            if (num < 0 || num > 9)
                return false;
            sum = 10 * sum + num;
        }
        if (isNegative)
            sum = -sum;
        *result = sum;
        return true;
    }

    bool String::Parse(const StringView& str, double* result)
    {
        double sum = 0.0;
        double fraction = 0.0;
        double fractionDivisor = 1.0;
        bool inFraction = false;
        bool isNegative = false;

        for (Char ch : str)
        {
            if (ch == L'-')
            {
                isNegative = true;
                continue;
            }
            if (ch == L'.')
            {
                if (inFraction)
                    return false;
                inFraction = true;
                continue;
            }

            uint64 num = ch - static_cast<uint64>('0');
            if (num < 0 || num > 9)
                return false;

            if (inFraction)
            {
                fraction = 10 * fraction + num;
                fractionDivisor *= 10;
            }
            else
                sum = 10 * sum + num;
        }

        if (inFraction)
            sum += fraction / fractionDivisor;
        if (isNegative)
            sum = -sum;
        *result = sum;
        return true;
    }

    String::String()
        : IString(0)
    {
    }

    String::String(uint64 capacity)
        : IString(0)
    {
        if (capacity > 0)
        {
            m_Data.Allocate(capacity);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Char* text)
        : IString(StringUtils::Length(text))
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), text, sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Char* text, uint64 length)
        : IString(length)
    {
        m_Length = length;
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), text, sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Slice<Char>& slice)
        : IString(slice.Length())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), slice.Get(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Iterator& begin, const Iterator& end)
        : IString(end.Ptr() - begin.Ptr())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), begin.Ptr(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const StringView& view)
        : IString(view.Length())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), view.Data(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const String& other)
        : IString(other.m_Length)
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), other.Data(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(String&& other)
        : IString(std::move(other))
    {
    }

    String& String::operator=(const String& text)
    {
        if (&text != this)
            Set(text);
        return *this;
    }

    String& String::operator=(const Char* text)
    {
        if (text != m_Data.Ptr())
            Set(text);
        return *this;
    }

    String& String::operator=(const StringView& text)
    {
        if (text.Data() != m_Data.Ptr())
            Set(text);
        return *this;
    }

    String& String::operator=(const Slice<Char>& slice)
    {
        if (slice.Get() != m_Data.Ptr())
            Set(slice);
        return *this;
    }

    String& String::operator=(String&& text)
    {
        if (&text != this)
        {
            Free();
            m_Data.Move(std::move(text.m_Data));
            m_Length = text.m_Length;

            text.m_Length = 0;
        }
        return *this;
    }

    String& String::operator+=(const String& text)
    {
        Append(text);
        return *this;
    }

    String& String::operator+=(const Char* text)
    {
        Append(text);
        return *this;
    }

    String& String::operator+=(Char ch)
    {
        Append(ch);
        return *this;
    }

    String& String::operator+=(const StringView& text)
    {
        Append(text);
        return *this;
    }

    String& String::operator+=(const Slice<Char>& slice)
    {
        Append(slice);
        return *this;
    }

    String& String::operator/=(const String& text)
    {
        Concat('/', text);
        return *this;
    }

    String& String::operator/=(const Char* text)
    {
        Concat('/', text);
        return *this;
    }

    String& String::operator/=(Char ch)
    {
        Concat('/', ch);
        return *this;
    }

    String& String::operator/=(const StringView& text)
    {
        Concat('/', text);
        return *this;
    }

    String& String::operator/=(const Slice<Char>& slice)
    {
        Concat('/', slice);
        return *this;
    }

    String String::operator+(const String& text)
    {
        String str(*this);
        str.Append(text);
        return str;
    }

    String String::operator+(const Char* text)
    {
        String str(*this);
        str.Append(text);
        return str;
    }

    String String::operator+(Char ch)
    {
        String str(*this);
        str.Append(ch);
        return str;
    }

    String String::operator+(const StringView& text)
    {
        String str(*this);
        str.Append(text);
        return str;
    }

    String String::operator+(const Slice<Char>& slice)
    {
        String str(*this);
        str.Append(slice);
        return str;
    }

    String String::operator/(const String& text)
    {
        String str(*this);
        str.Concat('/', text);
        return str;
    }

    String String::operator/(const Char* text)
    {
        String str(*this);
        str.Concat('/', text);
        return str;
    }

    String String::operator/(Char ch)
    {
        String str(*this);
        str.Concat('/', ch);
        return str;
    }

    String String::operator/(const StringView& text)
    {
        String str(*this);
        str.Concat('/', text);
        return str;
    }

    String String::operator/(const Slice<Char>& slice)
    {
        String str(*this);
        Concat('/', slice);
        return str;
    }

    bool String::operator==(const Char* str) const
    {
        return Compare(str);
    }

    bool String::operator==(const String& str) const
    {
        return Compare(str);
    }

    bool String::operator==(const StringView& str) const
    {
        return Compare(str);
    }

    bool String::operator!=(const Char* str) const
    {
        return !Compare(str);
    }

    bool String::operator!=(const String& str) const
    {
        return !Compare(str);
    }

    bool String::operator!=(const StringView& str) const
    {
        return !Compare(str);
    }

    String operator+(const Char* lhs, const String& rhs)
    {
        return String(lhs) + rhs;
    }

    String operator+(const StringView& lhs, const String& rhs)
    {
        return String(lhs) + rhs;
    }

    String operator/(const Char* lhs, const String& rhs)
    {
        return String(lhs) / rhs;
    }

    String operator/(const StringView& lhs, const String& rhs)
    {
        return String(lhs) / rhs;
    }

    void String::Set(const StringView& text)
    {
        if (!text.IsEmpty())
        {
            Resize(text.Length());
            Crt::Copy(m_Data.Ptr(), text.Data(), sizeof(Char) * text.Length());
            m_Length = text.Length();
            m_Data[m_Length] = 0;
        }
        else
            Free();
    }

    void String::Append(Char ch)
    {
        uint64 oldLength = m_Length;
        Resize(m_Length + 1);
        m_Data[oldLength] = ch;
        m_Data[m_Length] = 0;
    }

    void String::Append(const StringView& text)
    {
        if (!text.IsEmpty())
        {
            uint64 oldLength = m_Length;
            Resize(m_Length + text.Length());
            Crt::Copy(Data() + oldLength, text.Data(), sizeof(Char) * text.Length());
            m_Data[m_Length] = 0;
        }
    }

    void String::Concat(Char left, const StringView& text)
    {
        if (!text.IsEmpty())
        {
            uint64 oldLength = m_Length;
            Resize(m_Length + text.Length() + 1);
            m_Data[oldLength] = left;
            Crt::Copy(m_Data.Ptr() + oldLength + 1, text.Data(), sizeof(Char) * text.Length());
            m_Data[m_Length] = 0;
        }
    }

    void String::Concat(Char left, const String& text)
    {
        Concat(left, StringView(text));
    }

    void String::Concat(Char left, const Char* text)
    {
        Concat(left, StringView(text));
    }

    void String::Concat(Char left, Char ch)
    {
        uint64 oldLength = m_Length;
        Resize(m_Length + 2);
        m_Data[oldLength] = left;
        m_Data[oldLength + 1] = ch;
        m_Data[m_Length] = 0;
    }

    void String::Insert(uint64 index, const StringView& str)
    {
        ASSERT(index < m_Length);
        uint64 oldLength = m_Length;
        Resize(m_Length + str.Length());
        Crt::Move(m_Data.Ptr() + index + str.Length(), m_Data.Ptr() + index, sizeof(Char) * (oldLength - index));
        Crt::Copy(m_Data.Ptr() + index, str.Data(), sizeof(Char) * str.Length());
    }

    void String::Remove(uint64 index, uint64 length)
    {
        ASSERT(index + length < m_Length);
        Crt::Copy(m_Data.Ptr() + index, m_Data.Ptr() + index + length, sizeof(Char) * (m_Length - index + length));
        Resize(m_Length - length);
        m_Data[m_Length] = 0;
    }

    bool String::Compare(const StringView& str) const
    {
        if (m_Length == str.Length())
        {
            if (Crt::Compare(m_Data.Ptr(), str.Data(), sizeof(Char) * m_Length) == 0)
                return true;
        }
        return false;
    }

    String String::Substring(uint64 startIndex, uint64 length) const
    {
        ASSERT(startIndex + length < m_Length);
        String substr(length);
        Slice slice(ToSlice(*this, startIndex, length));
        substr.Set(slice);
        return substr;
    }

    String String::TrimTrailing() const
    {
        uint64 count = 0;
        for (uint64 i = m_Length; i > 0; --i)
        {
            if (m_Data[i - 1] != ' ')
                break;
            ++count;
        }
        if (count > 0)
        {
            return String(StringView(m_Data.Ptr(), m_Length - count));
        }
        return *this;
    }

    uint64 String::Find(const StringView& find) const
    {
        return StringView(*this).Find(find);
    }

    uint64 String::FindLast(const StringView& find) const
    {
        return StringView(*this).FindLast(find);
    }

    bool String::FindAll(const StringView& find, Array<uint64>& indices) const
    {
        return StringView(*this).FindAll(find, indices);
    }

    bool String::BeginsWith(const StringView& text) const
    {
        if (text.Length() < m_Length)
            return false;
        for (uint64 i = 0; i < text.Length(); ++i)
        {
            if (m_Data[i] != text[i])
                return false;
        }
        return true;
    }

    bool String::EndsWith(const StringView& text) const
    {
        if (text.Length() < m_Length)
            return false;
        for (uint64 i = text.Length(); i > 0; --i)
        {
            if (m_Data[i - 1] != text[i - 1])
                return false;
        }
        return true;
    }

    String String::Replace(uint64 index, uint64 length, const StringView& replace) const
    {
        String result;
        if (index + length <= m_Length)
        {
            Slice lhs(Begin(), Begin() + index);
            Slice rhs(Begin() + index + length, end());

            result.Append(lhs);
            result.Append(replace);
            result.Append(rhs);
        }
        return result;
    }

    String String::Replace(const StringView& find, const StringView& replace) const
    {
        String result = ReplaceFirst(find, replace);
        String next = result.ReplaceFirst(find, replace);
        while (next != Empty)
        {
            result = next;
            next = result.ReplaceFirst(find, replace);
        }
        return result;
    }

    String String::ReplaceFirst(const StringView& find, const StringView& replace) const
    {
        uint64 index = Find(find);
        if (index != GlobalVars::NoPos)
            return Replace(index, find.Length(), replace);
        return Empty;
    }

    String String::ReplaceLast(const StringView& find, const StringView& replace) const
    {
        uint64 index = FindLast(find);
        if (index != GlobalVars::NoPos)
            return Replace(index, find.Length(), replace);
        return Empty;
    }

    bool CharString::Parse(const CharStringView& str, int64* result)
    {
        bool isNegative = false;
        int64 sum = 0;
        for (char ch : str)
        {
            if (ch == L'-')
            {
                isNegative = true;
                continue;
            }

            uint64 num = ch - static_cast<uint64>('0');
            if (num < 0 || num > 9)
                return false;
            sum = 10 * sum + num;
        }
        if (isNegative)
            sum = -sum;
        *result = sum;
        return true;
    }

    bool CharString::Parse(const CharStringView& str, double* result)
    {
        double sum = 0.0;
        double fraction = 0.0;
        double fractionDivisor = 1.0;
        bool inFraction = false;
        bool isNegative = false;

        for (char ch : str)
        {
            if (ch == L'-')
            {
                isNegative = true;
                continue;
            }
            if (ch == L'.')
            {
                if (inFraction)
                    return false;
                inFraction = true;
                continue;
            }

            uint64 num = ch - static_cast<uint64>('0');
            if (num < 0 || num > 9)
                return false;

            if (inFraction)
            {
                fraction = 10 * fraction + num;
                fractionDivisor *= 10;
            }
            else
                sum = 10 * sum + num;
        }

        if (inFraction)
            sum += fraction / fractionDivisor;
        if (isNegative)
            sum = -sum;
        *result = sum;
        return true;
    }

    CharString::CharString()
        : IString(0)
    {
    }

    CharString::CharString(uint64 capacity)
        : IString(0)
    {
        if (capacity > 0)
        {
            m_Data.Allocate(capacity);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(const char* text)
        : IString(StringUtils::Length(text))
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), text, sizeof(char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(const char* text, uint64 length)
        : IString(length)
    {
        m_Length = length;
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), text, sizeof(char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(const Slice<char>& slice)
        : IString(slice.Length())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), slice.Get(), sizeof(char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(const Iterator& begin, const Iterator& end)
        : IString(end.Ptr() - begin.Ptr())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), begin.Ptr(), sizeof(char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(const CharStringView& view)
        : IString(view.Length())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), view.Data(), sizeof(char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(const CharString& other)
        : IString(other.m_Length)
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), other.Data(), sizeof(char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    CharString::CharString(CharString&& other)
        : IString(std::move(other))
    {
    }

    CharString& CharString::operator=(const CharString& text)
    {
        if (&text != this)
            Set(text);
        return *this;
    }

    CharString& CharString::operator=(const char* text)
    {
        if (text != m_Data.Ptr())
            Set(text);
        return *this;
    }

    CharString& CharString::operator=(const CharStringView& text)
    {
        if (text.Data() != m_Data.Ptr())
            Set(text);
        return *this;
    }

    CharString& CharString::operator=(const Slice<char>& slice)
    {
        if (slice.Get() != m_Data.Ptr())
            Set(slice);
        return *this;
    }

    CharString& CharString::operator=(CharString&& text)
    {
        if (&text != this)
        {
            Free();
            m_Data.Move(std::move(text.m_Data));
            m_Length = text.m_Length;

            text.m_Length = 0;
        }
        return *this;
    }

    CharString& CharString::operator+=(const CharString& text)
    {
        Append(text);
        return *this;
    }

    CharString& CharString::operator+=(const char* text)
    {
        Append(text);
        return *this;
    }

    CharString& CharString::operator+=(char ch)
    {
        Append(ch);
        return *this;
    }

    CharString& CharString::operator+=(const CharStringView& text)
    {
        Append(text);
        return *this;
    }

    CharString& CharString::operator+=(const Slice<char>& slice)
    {
        Append(slice);
        return *this;
    }

    CharString& CharString::operator/=(const CharString& text)
    {
        Concat('/', text);
        return *this;
    }

    CharString& CharString::operator/=(const char* text)
    {
        Concat('/', text);
        return *this;
    }

    CharString& CharString::operator/=(char ch)
    {
        Concat('/', ch);
        return *this;
    }

    CharString& CharString::operator/=(const CharStringView& text)
    {
        Concat('/', text);
        return *this;
    }

    CharString& CharString::operator/=(const Slice<char>& slice)
    {
        Concat('/', slice);
        return *this;
    }

    CharString CharString::operator+(const CharString& text)
    {
        CharString str(*this);
        str.Append(text);
        return str;
    }

    CharString CharString::operator+(const char* text)
    {
        CharString str(*this);
        str.Append(text);
        return str;
    }

    CharString CharString::operator+(char ch)
    {
        CharString str(*this);
        str.Append(ch);
        return str;
    }

    CharString CharString::operator+(const CharStringView& text)
    {
        CharString str(*this);
        str.Append(text);
        return str;
    }

    CharString CharString::operator+(const Slice<char>& slice)
    {
        CharString str(*this);
        str.Append(slice);
        return str;
    }

    CharString CharString::operator/(const CharString& text)
    {
        CharString str(*this);
        str.Concat('/', text);
        return str;
    }

    CharString CharString::operator/(const char* text)
    {
        CharString str(*this);
        str.Concat('/', text);
        return str;
    }

    CharString CharString::operator/(char ch)
    {
        CharString str(*this);
        str.Concat('/', ch);
        return str;
    }

    CharString CharString::operator/(const CharStringView& text)
    {
        CharString str(*this);
        str.Concat('/', text);
        return str;
    }

    CharString CharString::operator/(const Slice<char>& slice)
    {
        CharString str(*this);
        Concat('/', slice);
        return str;
    }

    bool CharString::operator==(const char* str) const
    {
        return Compare(str);
    }

    bool CharString::operator==(const CharString& str) const
    {
        return Compare(str);
    }

    bool CharString::operator==(const CharStringView& str) const
    {
        return Compare(str);
    }

    bool CharString::operator!=(const char* str) const
    {
        return !Compare(str);
    }

    bool CharString::operator!=(const CharString& str) const
    {
        return !Compare(str);
    }

    bool CharString::operator!=(const CharStringView& str) const
    {
        return !Compare(str);
    }

    CharString operator+(const char* lhs, const CharString& rhs)
    {
        return CharString(lhs) + rhs;
    }

    CharString operator+(const CharStringView& lhs, const CharString& rhs)
    {
        return CharString(lhs) + rhs;
    }

    CharString operator/(const char* lhs, const CharString& rhs)
    {
        return CharString(lhs) / rhs;
    }

    CharString operator/(const CharStringView& lhs, const CharString& rhs)
    {
        return CharString(lhs) / rhs;
    }

    void CharString::Set(const CharStringView& text)
    {
        if (!text.IsEmpty())
        {
            Resize(text.Length());
            Crt::Copy(m_Data.Ptr(), text.Data(), sizeof(char) * text.Length());
            m_Length = text.Length();
            m_Data[m_Length] = 0;
        }
        else
            Free();
    }

    void CharString::Append(char ch)
    {
        uint64 oldLength = m_Length;
        Resize(m_Length + 1);
        m_Data[oldLength] = ch;
        m_Data[m_Length] = 0;
    }

    void CharString::Append(const CharStringView& text)
    {
        if (!text.IsEmpty())
        {
            uint64 oldLength = m_Length;
            Resize(m_Length + text.Length());
            Crt::Copy(Data() + oldLength, text.Data(), sizeof(char) * text.Length());
            m_Data[m_Length] = 0;
        }
    }

    void CharString::Concat(char left, const CharStringView& text)
    {
        if (!text.IsEmpty())
        {
            uint64 oldLength = m_Length;
            Resize(m_Length + text.Length() + 1);
            m_Data[oldLength] = left;
            Crt::Copy(m_Data.Ptr() + oldLength + 1, text.Data(), sizeof(char) * text.Length());
            m_Data[m_Length] = 0;
        }
    }

    void CharString::Concat(char left, const CharString& text)
    {
        Concat(left, CharStringView(text));
    }

    void CharString::Concat(char left, const char* text)
    {
        Concat(left, CharStringView(text));
    }

    void CharString::Concat(char left, char ch)
    {
        uint64 oldLength = m_Length;
        Resize(m_Length + 2);
        m_Data[oldLength] = left;
        m_Data[oldLength + 1] = ch;
        m_Data[m_Length] = 0;
    }

    void CharString::Insert(uint64 index, const CharStringView& str)
    {
        ASSERT(index < m_Length);
        uint64 oldLength = m_Length;
        Resize(m_Length + str.Length());
        Crt::Move(m_Data.Ptr() + index + str.Length(), m_Data.Ptr() + index, sizeof(char) * (oldLength - index));
        Crt::Copy(m_Data.Ptr() + index, str.Data(), sizeof(char) * str.Length());
    }

    void CharString::Remove(uint64 index, uint64 length)
    {
        ASSERT(index + length < m_Length);
        Crt::Copy(m_Data.Ptr() + index, m_Data.Ptr() + index + length, sizeof(char) * (m_Length - index + length));
        Resize(m_Length - length);
        m_Data[m_Length] = 0;
    }

    bool CharString::Compare(const CharStringView& str) const
    {
        if (m_Length == str.Length())
        {
            if (Crt::Compare(m_Data.Ptr(), str.Data(), sizeof(char) * m_Length) == 0)
                return true;
        }
        return false;
    }

    CharString CharString::Substring(uint64 startIndex, uint64 length) const
    {
        ASSERT(startIndex + length < m_Length);
        CharString substr(length);
        Slice slice(ToSlice(*this, startIndex, length));
        substr.Set(slice);
        return substr;
    }

    CharString CharString::TrimTrailing() const
    {
        uint64 count = 0;
        for (uint64 i = m_Length; i > 0; --i)
        {
            if (m_Data[i - 1] != ' ')
                break;
            ++count;
        }
        if (count > 0)
        {
            return CharString(CharStringView(m_Data.Ptr(), m_Length - count));
        }
        return *this;
    }

    uint64 CharString::Find(const CharStringView& find) const
    {
        return CharStringView(*this).Find(find);
    }

    uint64 CharString::FindLast(const CharStringView& find) const
    {
        return CharStringView(*this).FindLast(find);
    }

    bool CharString::FindAll(const CharStringView& find, Array<uint64>& indices) const
    {
        return CharStringView(*this).FindAll(find, indices);
    }

    bool CharString::BeginsWith(const CharStringView& text) const
    {
        if (text.Length() < m_Length)
            return false;
        for (uint64 i = 0; i < text.Length(); ++i)
        {
            if (m_Data[i] != text[i])
                return false;
        }
        return true;
    }

    bool CharString::EndsWith(const CharStringView& text) const
    {
        if (text.Length() < m_Length)
            return false;
        for (uint64 i = text.Length(); i > 0; --i)
        {
            if (m_Data[i - 1] != text[i - 1])
                return false;
        }
        return true;
    }

    CharString CharString::Replace(uint64 index, uint64 length, const CharStringView& replace) const
    {
        CharString result;
        if (index + length <= m_Length)
        {
            Slice lhs(Begin(), Begin() + index);
            Slice rhs(Begin() + index + length, end());

            result.Append(lhs);
            result.Append(replace);
            result.Append(rhs);
        }
        return result;
    }

    CharString CharString::Replace(const CharStringView& find, const CharStringView& replace) const
    {
        CharString result = ReplaceFirst(find, replace);
        CharString next = result.ReplaceFirst(find, replace);
        while (next != Empty)
        {
            result = next;
            next = result.ReplaceFirst(find, replace);
        }
        return result;
    }

    CharString CharString::ReplaceFirst(const CharStringView& find, const CharStringView& replace) const
    {
        uint64 index = Find(find);
        if (index != GlobalVars::NoPos)
            return Replace(index, find.Length(), replace);
        return Empty;
    }

    CharString CharString::ReplaceLast(const CharStringView& find, const CharStringView& replace) const
    {
        uint64 index = FindLast(find);
        if (index != GlobalVars::NoPos)
            return Replace(index, find.Length(), replace);
        return Empty;
    }

    Slice<Char> ToSlice(const String& string)
    {
        return Slice(string.Begin(), string.end());
    }

    Slice<Char> ToSlice(const String& string, uint64 index, uint64 length)
    {
        return Slice(const_cast<Char*>(string.Data()) + index, length);
    }

    Slice<char> ToSlice(const CharString& string)
    {
        return Slice(string.Begin(), string.end());
    }

    Slice<char> ToSlice(const CharString& string, uint64 index, uint64 length)
    {
        return Slice(const_cast<char*>(string.Data()) + index, length);
    }
}
