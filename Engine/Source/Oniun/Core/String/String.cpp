#include "Oniun.pch.h"
#include "Oniun/Core/String/String.h"

#include "Oniun/Core/String/StringUtils.h"

const String String::Empty = String();

bool String::Parse(const StringView& str, int64* result)
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

bool String::Parse(const StringView& str, double* result)
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

String::String()
    : m_Length(0)
{
}

String::String(uint64 capacity)
    : m_Length(0)
{
    if (capacity > 0)
    {
        m_Data.Allocate(capacity);
        m_Data[m_Length] = 0;
    }
}

String::String(const char* text)
    : m_Length(StringUtils::Length(text))
{
    if (m_Length > 0)
    {
        m_Data.Allocate(m_Length + 1);
        Crt::Copy(m_Data.Ptr(), text, m_Length);
        m_Data[m_Length] = 0;
    }
}

String::String(const char* text, uint64 length)
    : m_Length(length)
{
    m_Length = length;
    if (m_Length > 0)
    {
        m_Data.Allocate(m_Length + 1);
        Crt::Copy(Data(), text, m_Length);
        m_Data[m_Length] = 0;
    }
}

String::String(const Slice<char>& slice)
    : m_Length(slice.Length())
{
    if (m_Length > 0)
    {
        m_Data.Allocate(m_Length + 1);
        Crt::Copy(Data(), slice.Get(), m_Length);
        m_Data[m_Length] = 0;
    }
}

String::String(const Iterator& begin, const Iterator& end)
    : m_Length(end.Ptr() - begin.Ptr())
{
    if (m_Length > 0)
    {
        m_Data.Allocate(m_Length + 1);
        Crt::Copy(Data(), begin.Ptr(), m_Length);
        m_Data[m_Length] = 0;
    }
}

String::String(const StringView& view)
    : m_Length(view.Length())
{
    if (m_Length > 0)
    {
        m_Data.Allocate(m_Length + 1);
        Crt::Copy(Data(), view.Data(), m_Length);
        m_Data[m_Length] = 0;
    }
}

String::String(const String& other)
    : m_Length(other.m_Length)
{
    if (m_Length > 0)
    {
        m_Data.Allocate(m_Length + 1);
        Crt::Copy(Data(), other.Data(), m_Length);
        m_Data[m_Length] = 0;
    }
}

String::String(String&& other)
    : m_Length(other.m_Length), m_Data(Memory::Move(other.m_Data))
{
}

String& String::operator=(const String& text)
{
    if (&text != this)
        Set(text);
    return *this;
}

String& String::operator=(const char* text)
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

String& String::operator=(const Slice<char>& slice)
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
        m_Data.Move(Memory::Move(text.m_Data));
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

String& String::operator+=(const char* text)
{
    Append(text);
    return *this;
}

String& String::operator+=(char ch)
{
    Append(ch);
    return *this;
}

String& String::operator+=(const StringView& text)
{
    Append(text);
    return *this;
}

String& String::operator+=(const Slice<char>& slice)
{
    Append(slice);
    return *this;
}

String& String::operator/=(const String& text)
{
    Concat('/', text);
    return *this;
}

String& String::operator/=(const char* text)
{
    Concat('/', text);
    return *this;
}

String& String::operator/=(char ch)
{
    Concat('/', ch);
    return *this;
}

String& String::operator/=(const StringView& text)
{
    Concat('/', text);
    return *this;
}

String& String::operator/=(const Slice<char>& slice)
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

String String::operator+(const char* text)
{
    String str(*this);
    str.Append(text);
    return str;
}

String String::operator+(char ch)
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

String String::operator+(const Slice<char>& slice)
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

String String::operator/(const char* text)
{
    String str(*this);
    str.Concat('/', text);
    return str;
}

String String::operator/(char ch)
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

String String::operator/(const Slice<char>& slice)
{
    String str(*this);
    Concat('/', slice);
    return str;
}

bool String::operator==(const char* str) const
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

bool String::operator!=(const char* str) const
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

String operator+(const char* lhs, const String& rhs)
{
    return String(lhs) + rhs;
}

String operator+(const StringView& lhs, const String& rhs)
{
    return String(lhs) + rhs;
}

String operator/(const char* lhs, const String& rhs)
{
    return String(lhs) / rhs;
}

String operator/(const StringView& lhs, const String& rhs)
{
    return String(lhs) / rhs;
}

void String::Clear()
{
    m_Length = 0;
}

void String::Free()
{
    if (m_Data.Ptr())
    {
        m_Data.Free();
        m_Length = 0;
    }
}

void String::Resize(uint64 newLength)
{
    if (newLength >= Capacity())
        Reserve(Memory::CalcCapacityGrow(newLength, m_Data.Capacity()));
    m_Length = newLength;
}

void String::Reserve(uint64 newCapacity, bool preserveContents)
{
    uint64 length = preserveContents ? (m_Length < newCapacity ? m_Length : newCapacity) : 0;
    m_Data.Relocate(newCapacity, m_Length, length);
}

void String::ReCalcLength()
{
    m_Length = StringUtils::Length(m_Data.Ptr());
}

void String::CorrectPathSlashes()
{
    for (uint64 i = 0; i < m_Length; ++i)
    {
        if (m_Data[i] == '\\')
            m_Data[i] = '/';
    }
}

void String::CorrectPlatformPathSlashes()
{
    for (uint64 i = 0; i < m_Length; ++i)
    {
#if ONU_PLATFORM_WINDOWS
        if (m_Data[i] == '/')
            m_Data[i] = '\\';
#else
            if (m_Data[i] == '\\')
                m_Data[i] = '/';
#endif
    }
}

void String::Set(const StringView& text)
{
    if (!text.IsEmpty())
    {
        Resize(text.Length());
        Crt::Copy(m_Data.Ptr(), text.Data(), text.Length());
        m_Length = text.Length();
        m_Data[m_Length] = 0;
    }
    else
        Free();
}

void String::Append(char ch)
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
        Crt::Copy(Data() + oldLength, text.Data(), text.Length());
        m_Data[m_Length] = 0;
    }
}

void String::Concat(char left, const StringView& text)
{
    if (!text.IsEmpty())
    {
        uint64 oldLength = m_Length;
        Resize(m_Length + text.Length() + 1);
        m_Data[oldLength] = left;
        Crt::Copy(m_Data.Ptr() + oldLength + 1, text.Data(), text.Length());
        m_Data[m_Length] = 0;
    }
}

void String::Concat(char left, const String& text)
{
    Concat(left, StringView(text));
}

void String::Concat(char left, const char* text)
{
    Concat(left, StringView(text));
}

void String::Concat(char left, char ch)
{
    uint64 oldLength = m_Length;
    Resize(m_Length + 2);
    m_Data[oldLength] = left;
    m_Data[oldLength + 1] = ch;
    m_Data[m_Length] = 0;
}

void String::Insert(uint64 index, const StringView& str)
{
    DEBUG_ASSERT(index < m_Length);
    uint64 oldLength = m_Length;
    Resize(m_Length + str.Length());
    Crt::Move(m_Data.Ptr() + index + str.Length(), m_Data.Ptr() + index, (oldLength - index));
    Crt::Copy(m_Data.Ptr() + index, str.Data(), str.Length());
}

void String::Remove(uint64 index, uint64 length)
{
    DEBUG_ASSERT(index + length < m_Length);
    Crt::Copy(m_Data.Ptr() + index, m_Data.Ptr() + index + length, (m_Length - index + length));
    Resize(m_Length - length);
    m_Data[m_Length] = 0;
}

bool String::Compare(const StringView& str) const
{
    if (m_Length == str.Length())
    {
        if (Crt::Compare(m_Data.Ptr(), str.Data(), m_Length) == 0)
            return true;
    }
    return false;
}

String String::Substring(uint64 startIndex, uint64 length) const
{
    DEBUG_ASSERT(startIndex + length < m_Length);
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

uint64 String::Find(const StringView& find, StringSearch opt, uint64 offset) const
{
    if (opt == StringSearch::CaseSensitive)
        return StringUtils::Find(m_Data.Ptr(), m_Length, find.Data(), find.Length(), offset);
    return StringUtils::FindIgnoreCase(m_Data.Ptr(), m_Length, find.Data(), find.Length(), offset);
}

uint64 String::FindLast(const StringView& find, StringSearch opt, uint64 offset) const
{
    if (opt == StringSearch::CaseSensitive)
        return StringUtils::FindLast(m_Data.Ptr(), m_Length, find.Data(), find.Length(), offset);
    return StringUtils::FindLastIgnoreCase(m_Data.Ptr(), m_Length, find.Data(), find.Length(), offset);
}

bool String::FindAll(const StringView& find, Array<uint64>& indices) const
{
    return StringView(*this).FindAll(find, indices);
}

bool String::BeginsWith(const StringView& text) const
{
    return StringUtils::BeginsWith(m_Data.Ptr(), m_Length, text.Data(), text.Length());
}

bool String::EndsWith(const StringView& text) const
{
    return StringUtils::EndsWith(m_Data.Ptr(), m_Length, text.Data(), text.Length());
}

uint64 String::ReplaceChars(const char* search, uint64 searchLength, const char* replace, uint64 replaceLength,
                            bool findFirst, StringSearch opt, uint64 offset)
{
    if (m_Length == 0 || searchLength == 0 || searchLength > m_Length)
        return NO_POS;

    uint64 index;
    if (opt == StringSearch::CaseSensitive)
        index = findFirst
                    ? StringUtils::Find(m_Data.Ptr(), m_Length, search, searchLength, offset)
                    : StringUtils::FindLast(m_Data.Ptr(), m_Length, search, searchLength, offset);
    else
        index = findFirst
                    ? StringUtils::FindIgnoreCase(m_Data.Ptr(), m_Length, search, searchLength, offset)
                    : StringUtils::FindLastIgnoreCase(m_Data.Ptr(), m_Length, search, searchLength, offset);

    if (index != NO_POS)
    {
        if (searchLength == replaceLength)
            Crt::Copy(m_Data.Ptr() + index, replace, replaceLength);
        else
        {
            uint64 oldLength = m_Length;
            Resize(m_Length - searchLength + replaceLength);
            Crt::Move(m_Data.Ptr() + index + replaceLength, m_Data.Ptr() + index + searchLength,
                      (oldLength - index - searchLength));
            Crt::Copy(m_Data.Ptr() + index, replace, replaceLength);
            m_Data[m_Length] = '\0';
        }
    }
    return index;
}

void String::Replace(const StringView& find, const StringView& replace, StringSearch opt)
{
    uint64 index = ReplaceChars(find.Data(), find.Length(), replace.Data(), replace.Length(), true, opt);
    while (index != NO_POS)
        index = ReplaceChars(find.Data(), find.Length(), replace.Data(), replace.Length(), true, opt,
                             index + find.Length());
}

void String::ReplaceFirst(const StringView& find, const StringView& replace, StringSearch opt)
{
    ReplaceChars(find.Data(), find.Length(), replace.Data(), replace.Length(), true, opt);
}

void String::ReplaceLast(const StringView& find, const StringView& replace, StringSearch opt)
{
    ReplaceChars(find.Data(), find.Length(), replace.Data(), replace.Length(), false, opt);
}

Slice<char> ToSlice(const String& string)
{
    return Slice(string.Begin(), string.end());
}

Slice<char> ToSlice(const String& string, uint64 index, uint64 length)
{
    return Slice(const_cast<char*>(string.Data()) + index, length);
}
