#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/UStringUtils.h"
#include "Oniun/Core/Containers/Array.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/Types/PackedIterator.h"
#include "Oniun/Core/Types/Slice.h"

namespace Onu
{
    class String
    {
    public:
        using Allocator = HeapAllocation::Data<Char>;
        using Iterator = PackedIterator<Char>;

    private:
        Allocator m_Data;
        uint64 m_Length;

    public:
        static constexpr uint64 NoPos = MAX_UINT64;
        static const String Empty;

        static constexpr Char UnixPathSeparator = L'/';
        static constexpr Char WindowsPathSeparator = L'\\';
        static constexpr Char PlatformPathSeparator = ONU_PLATFORM_WINDOWS ? WindowsPathSeparator : UnixPathSeparator;

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

        ~String();

        String& operator=(const String& text);
        String& operator=(const Char* text);
        String& operator=(const StringView& text);
        String& operator=(const Slice<Char>& slice);
        String& operator=(String&& text);

        String& operator+=(const String& text);
        String& operator+=(const Char* text);
        String& operator+=(const StringView& text);
        String& operator+=(const Slice<Char>& slice);

        String& operator/=(const String& text);
        String& operator/=(const Char* text);
        String& operator/=(const StringView& text);
        String& operator/=(const Slice<Char>& slice);

        String operator+(const String& text);
        String operator+(const Char* text);
        String operator+(const StringView& text);
        String operator+(const Slice<Char>& slice);

        String operator/(const String& text);
        String operator/(const Char* text);
        String operator/(const StringView& text);
        String operator/(const Slice<Char>& slice);

        Char* operator*();
        const Char* operator*() const;

        friend String operator+(const Char* lhs, const String& rhs);
        friend String operator+(const StringView& lhs, const String& rhs);
        friend String operator/(const Char* lhs, const String& rhs);
        friend String operator/(const StringView& lhs, const String& rhs);

        Char& operator[](uint64 index);
        const Char& operator[](uint64 index) const;

        bool operator==(const Char* str) const;
        bool operator==(const String& str) const;
        bool operator==(const StringView& str) const;
        bool operator!=(const Char* str) const;
        bool operator!=(const String& str) const;
        bool operator!=(const StringView& str) const;

        FORCE_INLINE Char* Data()
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE const Char* Data() const
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
            return Iterator(const_cast<Char*>(m_Data.Ptr()));
        }

        FORCE_INLINE Iterator end()
        {
            return Iterator(m_Data.Ptr() + m_Length);
        }

        FORCE_INLINE Iterator end() const
        {
            return Iterator(const_cast<Char*>(m_Data.Ptr()) + m_Length);
        }

        FORCE_INLINE bool IsEmpty()
        {
            return m_Length == 0 || m_Length == NoPos || !m_Data.Ptr();
        }

        void Clear();
        void Free();
        void Resize(uint64 newLength);
        void Reserve(uint64 newCapacity, bool preserveContents = true);

        void Set(const StringView& text);
        void Append(const StringView& text);

        void Concat(const Char& left, const StringView& text);
        void Concat(const Char& left, const String& text);
        void Concat(const Char& left, const Char* text);

        template<typename... TArgs>
        void Concat(const Char& left, const TArgs&... args);

        void Insert(uint64 index, const Char* text, uint64 length = NoPos);
        void Insert(uint64 index, const String& text);

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

    class StringView
    {
    public:
        using Iterator = PackedIterator<Char>;

    private:
        const Char* m_Data;
        uint64 m_Length;

    public:
        static constexpr uint64 NoPos = MAX_UINT64;

        constexpr StringView()
            : m_Data(nullptr), m_Length(0)
        {
        }

        constexpr StringView(const Char* text)
            : m_Data(text), m_Length(UStringUtils::Length(text))
        {
        }

        constexpr StringView(const Char* text, uint64 length)
            : m_Data(text), m_Length(length)
        {
        }

        StringView(const String& text)
            : m_Data(text.Data()), m_Length(text.Length())
        {
        }

        constexpr StringView(const Slice<Char>& slice)
            : m_Data(slice.Get()), m_Length(slice.Length())
        {
        }

        constexpr StringView(const Slice<const Char>& slice)
            : m_Data(slice.Get()), m_Length(slice.Length())
        {
        }

        constexpr const Char* operator*() const
        {
            return m_Data;
        }

        constexpr const Char& operator[](uint64 index) const
        {
            return m_Data[index];
        }

        constexpr const Char* Data() const
        {
            return m_Data;
        }

        constexpr uint64 Length() const
        {
            return m_Length;
        }

        constexpr Iterator begin() const
        {
            return Iterator(const_cast<Char*>(m_Data));
        }

        constexpr Iterator end() const
        {
            return Iterator(const_cast<Char*>(m_Data + m_Length));
        }

        constexpr bool IsEmpty() const
        {
            return m_Length == 0 || m_Length == NoPos || !m_Data;
        }

        constexpr uint64 Find(const StringView& find, uint64 offset = 0) const
        {
            for (uint64 i = offset; i < m_Length - find.Length() + 1; ++i)
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
                    return i;
            }
            return NoPos;
        }

        constexpr uint64 FindLast(const StringView& find, uint64 offset = 0) const
        {
            for (uint64 i = m_Length - find.Length() - offset; i > 0; --i)
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
                    return i;
            }
            return NoPos;
        }

        bool FindAll(const StringView& find, Array<uint64>& indices) const
        {
            indices.Clear();
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

        constexpr bool BeginsWith(const StringView& text) const
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

        constexpr bool EndsWith(const StringView& text) const
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
    };

    constexpr Slice<Char> ToSlice(const StringView& string)
    {
        return Slice(const_cast<Char*>(string.Data()), string.Length());
    }

    constexpr Slice<Char> ToSlice(const StringView& string, uint64 index, uint64 length)
    {
        return Slice(const_cast<Char*>(string.Data()) + index, length);
    }

    template<typename... TArgs>
    void String::Concat(const Char& left, const TArgs&... args)
    {
        Concat(left, args...);
    }

    Slice<Char> ToSlice(const String& string);
    Slice<Char> ToSlice(const String& string, uint64 index, uint64 length);
}
