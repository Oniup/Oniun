#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/Templates/PackedIterator.h"
#include "Oniun/Core/Templates/Slice.h"

namespace Onu
{
    template<typename TChar>
    class IString
    {
    public:
        using CharType = TChar;
        using Allocator = HeapAllocation::Data<TChar>;
        using Iterator = PackedIterator<TChar>;

    public:
        static constexpr uint64 NoPos = MAX_UINT64;

    protected:
        uint64 m_Length;
        Allocator m_Data;

    public:
        IString(uint64 length)
            : m_Length(length)
        {
        }

        IString(IString&& str)
            : m_Length(str.m_Length), m_Data(std::move(str.m_Data))
        {
            str.m_Length = 0;
        }

        virtual ~IString()
        {
            m_Length = 0;
        }

        FORCE_INLINE TChar* operator*()
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE const TChar* operator*() const
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE Char& operator[](uint64 index)
        {
            ASSERT(index < m_Length);
            return m_Data[index];
        }

        FORCE_INLINE const Char& operator[](uint64 index) const
        {
            ASSERT(index < m_Length);
            return m_Data[index];
        }

        FORCE_INLINE TChar* Data()
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE const TChar* Data() const
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE uint64 Length() const
        {
            return m_Length;
        }

        FORCE_INLINE uint64 Capacity() const
        {
            return m_Data.Capacity();
        }

        FORCE_INLINE Iterator begin()
        {
            return Iterator(m_Data.Ptr());
        }

        FORCE_INLINE Iterator begin() const
        {
            return Iterator(const_cast<TChar*>(m_Data.Ptr()));
        }

        FORCE_INLINE Iterator end()
        {
            return Iterator(m_Data.Ptr() + m_Length);
        }

        FORCE_INLINE Iterator end() const
        {
            return Iterator(const_cast<TChar*>(m_Data.Ptr()) + m_Length);
        }

        FORCE_INLINE bool IsEmpty()
        {
            return m_Length == 0 || m_Length == NoPos || !m_Data.Ptr();
        }

        void Clear()
        {
            m_Length = 0;
        }

        void Free()
        {
            if (m_Data.Ptr())
            {
                m_Data.Free();
                m_Length = 0;
            }
        }

        void Resize(uint64 newLength)
        {
            Reserve(Memory::CalcCapacityGrow(newLength, m_Data.Capacity()));
            m_Length = newLength;
        }

        void Reserve(uint64 newCapacity, bool preserveContents = true)
        {
            uint64 length = preserveContents ? (m_Length < newCapacity ? m_Length : newCapacity) : 0;
            m_Data.Relocate(newCapacity, m_Length, length);
        }

        void ReCalcLength()
        {
            m_Length = StringUtils::Length(m_Data.Ptr());
        }
    };

    class String : public IString<Char>
    {
        friend StringView;

    public:
        static const String Empty;

        static constexpr Char UnixPathSeparator = L'/';
        static constexpr Char WindowsPathSeparator = L'\\';
        static constexpr Char PlatformPathSeparator = ONU_PLATFORM_WINDOWS ? WindowsPathSeparator : UnixPathSeparator;

    public:
        static bool Parse(const StringView& str, int64* result);
        static bool Parse(const StringView& str, double* result);


        String();
        String(uint64 capacity);
        String(const Char* text);
        String(const Char* text, uint64 length);
        String(const Slice<Char>& slice);
        String(const Iterator& begin, const Iterator& end);
        String(const StringView& view);
        String(const String& other);
        String(String&& other);

        String& operator=(const String& text);
        String& operator=(const Char* text);
        String& operator=(const StringView& text);
        String& operator=(const Slice<Char>& slice);
        String& operator=(String&& text);

        String& operator+=(const String& text);
        String& operator+=(const Char* text);
        String& operator+=(const Char& ch);
        String& operator+=(const StringView& text);
        String& operator+=(const Slice<Char>& slice);

        String& operator/=(const String& text);
        String& operator/=(const Char* text);
        String& operator/=(const Char& ch);
        String& operator/=(const StringView& text);
        String& operator/=(const Slice<Char>& slice);

        String operator+(const String& text);
        String operator+(const Char* text);
        String operator+(const Char& ch);
        String operator+(const StringView& text);
        String operator+(const Slice<Char>& slice);

        String operator/(const String& text);
        String operator/(const Char* text);
        String operator/(const Char& ch);
        String operator/(const StringView& text);
        String operator/(const Slice<Char>& slice);

        friend String operator+(const Char* lhs, const String& rhs);
        friend String operator+(const StringView& lhs, const String& rhs);
        friend String operator/(const Char* lhs, const String& rhs);
        friend String operator/(const StringView& lhs, const String& rhs);

        bool operator==(const Char* str) const;
        bool operator==(const String& str) const;
        bool operator==(const StringView& str) const;
        bool operator!=(const Char* str) const;
        bool operator!=(const String& str) const;
        bool operator!=(const StringView& str) const;

        void Set(const StringView& text);
        void Append(const Char& ch);
        void Append(const StringView& text);

        void Concat(const Char& left, const StringView& text);
        void Concat(const Char& left, const String& text);
        void Concat(const Char& left, const Char* text);
        void Concat(const Char& left, const Char& ch);

        template<typename... TArgs>
        void Concat(const Char& left, const TArgs&... args);

        void Insert(uint64 index, const StringView& str);

        void Remove(uint64 index, uint64 length);

        bool Compare(const StringView& str) const;

        String Substring(uint64 startIndex, uint64 length = NoPos) const;
        String TrimTrailing() const;

        uint64 Find(const StringView& find) const;
        uint64 FindLast(const StringView& find) const;
        bool FindAll(const StringView& find, Array<uint64>& indices) const;

        bool BeginsWith(const StringView& text) const;
        bool EndsWith(const StringView& text) const;

        String Replace(uint64 index, uint64 length, const StringView& replace) const;
        String Replace(const StringView& find, const StringView& replace) const;
        String ReplaceFirst(const StringView& find, const StringView& replace) const;
        String ReplaceLast(const StringView& find, const StringView& replace) const;
    };

    class CharString : public IString<char>
    {
        friend CharStringView;

    public:
        static const CharString Empty;

        static constexpr char UnixPathSeparator = '/';
        static constexpr char WindowsPathSeparator = '\\';
        static constexpr char PlatformPathSeparator = ONU_PLATFORM_WINDOWS ? WindowsPathSeparator : UnixPathSeparator;

    public:
        CharString();
        CharString(uint64 capacity);
        CharString(const char* text);
        CharString(const char* text, uint64 length);
        CharString(const Slice<char>& slice);
        CharString(const Iterator& begin, const Iterator& end);
        CharString(const CharStringView& view);
        CharString(const CharString& other);
        CharString(CharString&& other);

        CharString& operator=(const CharString& text);
        CharString& operator=(const char* text);
        CharString& operator=(const CharStringView& text);
        CharString& operator=(const Slice<char>& slice);
        CharString& operator=(CharString&& text);

        CharString& operator+=(const CharString& text);
        CharString& operator+=(const char* text);
        CharString& operator+=(const char& ch);
        CharString& operator+=(const CharStringView& text);
        CharString& operator+=(const Slice<char>& slice);

        CharString& operator/=(const CharString& text);
        CharString& operator/=(const char* text);
        CharString& operator/=(const char& ch);
        CharString& operator/=(const CharStringView& text);
        CharString& operator/=(const Slice<char>& slice);

        CharString operator+(const CharString& text);
        CharString operator+(const char* text);
        CharString operator+(const char& ch);
        CharString operator+(const CharStringView& text);
        CharString operator+(const Slice<char>& slice);

        CharString operator/(const CharString& text);
        CharString operator/(const char* text);
        CharString operator/(const char& ch);
        CharString operator/(const CharStringView& text);
        CharString operator/(const Slice<char>& slice);

        friend CharString operator+(const char* lhs, const CharString& rhs);
        friend CharString operator+(const CharStringView& lhs, const CharString& rhs);
        friend CharString operator/(const char* lhs, const CharString& rhs);
        friend CharString operator/(const CharStringView& lhs, const CharString& rhs);

        bool operator==(const char* str) const;
        bool operator==(const CharString& str) const;
        bool operator==(const CharStringView& str) const;
        bool operator!=(const char* str) const;
        bool operator!=(const CharString& str) const;
        bool operator!=(const CharStringView& str) const;

        void Set(const CharStringView& text);
        void Append(const char& ch);
        void Append(const CharStringView& text);

        void Concat(const char& left, const CharStringView& text);
        void Concat(const char& left, const CharString& text);
        void Concat(const char& left, const char* text);
        void Concat(const char& left, const char& ch);

        template<typename... TArgs>
        void Concat(const char& left, const TArgs&... args);

        void Insert(uint64 index, const CharStringView& str);

        void Remove(uint64 index, uint64 length);

        bool Compare(const CharStringView& str) const;

        CharString Substring(uint64 startIndex, uint64 length = NoPos) const;
        CharString TrimTrailing() const;

        uint64 Find(const CharStringView& find) const;
        uint64 FindLast(const CharStringView& find) const;
        bool FindAll(const CharStringView& find, Array<uint64>& indices) const;

        bool BeginsWith(const CharStringView& text) const;
        bool EndsWith(const CharStringView& text) const;

        CharString Replace(uint64 index, uint64 length, const CharStringView& replace) const;
        CharString Replace(const CharStringView& find, const CharStringView& replace) const;
        CharString ReplaceFirst(const CharStringView& find, const CharStringView& replace) const;
        CharString ReplaceLast(const CharStringView& find, const CharStringView& replace) const;

        static bool Parse(const CharStringView& str, int64* result);
        static bool Parse(const CharStringView& str, double* result);
    };

    template<typename... TArgs>
    void String::Concat(const Char& left, const TArgs&... args)
    {
        // TODO: ...
    }

    template<typename... TArgs>
    void CharString::Concat(const char& left, const TArgs&... args)
    {
        // TODO: ...
    }

    Slice<Char> ToSlice(const String& string);
    Slice<Char> ToSlice(const String& string, uint64 index, uint64 length);

    Slice<char> ToSlice(const CharString& string);
    Slice<char> ToSlice(const CharString& string, uint64 index, uint64 length);
}
