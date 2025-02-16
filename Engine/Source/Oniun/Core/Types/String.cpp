#include "Oniun/Core/Types/String.h"

namespace Onu
{
    const String String::Empty = String();

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

    String::String(const Char* text)
        : m_Length(UStringUtils::Length(text))
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), text, sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Char* text, uint64 length)
        : m_Length(length)
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), text, sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Slice<Char>& slice)
        : m_Length(slice.Length())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), slice.Get(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const Iterator& begin, const Iterator& end)
        : m_Length(end.Ptr() - begin.Ptr())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), begin.Ptr(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const StringView& view)
        : m_Length(view.Length())
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), view.Data(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(const String& other)
        : m_Length(other.m_Length)
    {
        if (m_Length > 0)
        {
            m_Data.Allocate(m_Length + 1);
            Crt::Copy(Data(), other.Data(), sizeof(Char) * m_Length);
            m_Data[m_Length] = 0;
        }
    }

    String::String(String&& other)
        : m_Data(std::move(other.m_Data)), m_Length(other.m_Length)
    {
        other.m_Length = 0;
    }

    String::~String()
    {
        m_Length = 0;
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
        Concat(PlatformPathSeparator, text);
        return *this;
    }

    String& String::operator/=(const Char* text)
    {
        Concat(PlatformPathSeparator, text);
        return *this;
    }

    String& String::operator/=(const StringView& text)
    {
        Concat(PlatformPathSeparator, text);
        return *this;
    }

    String& String::operator/=(const Slice<Char>& slice)
    {
        Concat(PlatformPathSeparator, slice);
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
        str.Concat(PlatformPathSeparator, text);
        return str;
    }

    String String::operator/(const Char* text)
    {
        String str(*this);
        str.Concat(PlatformPathSeparator, text);
        return str;
    }

    String String::operator/(const StringView& text)
    {
        String str(*this);
        str.Concat(PlatformPathSeparator, text);
        return str;
    }

    String String::operator/(const Slice<Char>& slice)
    {
        String str(*this);
        Concat(PlatformPathSeparator, slice);
        return str;
    }

    Char* String::operator*()
    {
        return m_Data.Ptr();
    }

    const Char* String::operator*() const
    {
        return m_Data.Ptr();
    }

    Char& String::operator[](uint64 index)
    {
        ASSERT(index < m_Length);
        return m_Data[index];
    }

    const Char& String::operator[](uint64 index) const
    {
        ASSERT(index < m_Length);
        return m_Data[index];
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
        Reserve(Memory::CalcCapacityGrow(newLength, m_Data.Capacity()));
        m_Length = newLength;
    }

    void String::Reserve(uint64 newCapacity, bool preserveContents)
    {
        uint64 length = preserveContents ? (m_Length < newCapacity ? m_Length : newCapacity) : 0;
        m_Data.Relocate(newCapacity, m_Length, length);
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

    void String::Concat(const Char& left, const StringView& text)
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
    
    void String::Concat(const Char& left, const String& text)
    {
        Concat(left, StringView(text));
    }

    void String::Concat(const Char& left, const Char* text)
    {
        Concat(left, StringView(text));
    }

    void String::Insert(uint64 index, const Char* text, uint64 length)
    {
        ASSERT(index < m_Length);
        uint64 oldLength = m_Length;
        if (length == NoPos)
            length = UStringUtils::Length(text);
        
        Resize(m_Length + length);
        Crt::Move(m_Data.Ptr() + index + length, m_Data.Ptr() + index, sizeof(Char) * (oldLength - index));
        Crt::Copy(m_Data.Ptr() + index, text, sizeof(Char) * length);
    }

    void String::Insert(uint64 index, const String& text)
    {
        Insert(index, text.Data(), text.Length());
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

    uint64 String::Find(const StringView& text) const
    {
        for (uint64 i = 0; i <= m_Length - text.Length(); ++i)
        {
            bool found = true;
            for (uint64 j = 0; j < text.Length(); ++j)
            {
                if (m_Data[i + j] != text[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return NoPos;
    }

    uint64 String::FindLast(const StringView& text) const
    {
        for (uint64 i = m_Length - text.Length(); i > 0; --i)
        {
            bool found = true;
            for (uint64 j = 0; j < text.Length(); ++j)
            {
                if (m_Data[i + j] != text[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return NoPos;
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
            Slice lhs(m_Data.Ptr(), index);
            Slice rhs(begin() + index + length, end());
            
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
        if (index != NoPos)
            return Replace(index, find.Length(), replace);
        return Empty;
    }

    String String::ReplaceLast(const StringView& find, const StringView& replace) const
    {
        uint64 index = FindLast(find);
        if (index != NoPos)
            return Replace(index, find.Length(), replace);
        return Empty;
    }

    Slice<Char> ToSlice(const String& string)
    {
        return Slice(string.begin(), string.end());
    }
    
    Slice<Char> ToSlice(const String& string, uint64 index, uint64 length)
    {
        return Slice(const_cast<Char*>(string.Data()) + index, length);
    }
}
