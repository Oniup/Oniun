#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/Templates/PackedIterator.h"
#include "Oniun/Core/Templates/Slice.h"

namespace Oniun
{
    class String
    {
        friend StringView;

    public:
        using Allocator = HeapAllocation::Data<char>;
        using Iterator = PackedIterator<char>;

        static const String Empty;

    public:
        static bool Parse(const StringView& str, int64* result);
        static bool Parse(const StringView& str, double* result);

    public:
        String();
        String(uint64 capacity);
        String(const char* text);
        String(const char* text, uint64 length);
        String(const Slice<char>& slice);
        String(const Iterator& begin, const Iterator& end);
        String(const StringView& view);
        String(const String& other);
        String(String&& other);

    public:
        String& operator=(const String& text);
        String& operator=(const char* text);
        String& operator=(const StringView& text);
        String& operator=(const Slice<char>& slice);
        String& operator=(String&& text);

        String& operator+=(const String& text);
        String& operator+=(const char* text);
        String& operator+=(char ch);
        String& operator+=(const StringView& text);
        String& operator+=(const Slice<char>& slice);

        String& operator/=(const String& text);
        String& operator/=(const char* text);
        String& operator/=(char ch);
        String& operator/=(const StringView& text);
        String& operator/=(const Slice<char>& slice);

        String operator+(const String& text);
        String operator+(const char* text);
        String operator+(char ch);
        String operator+(const StringView& text);
        String operator+(const Slice<char>& slice);

        String operator/(const String& text);
        String operator/(const char* text);
        String operator/(char ch);
        String operator/(const StringView& text);
        String operator/(const Slice<char>& slice);

        friend String operator+(const char* lhs, const String& rhs);
        friend String operator+(const StringView& lhs, const String& rhs);
        friend String operator/(const char* lhs, const String& rhs);
        friend String operator/(const StringView& lhs, const String& rhs);

        bool operator==(const char* str) const;
        bool operator==(const String& str) const;
        bool operator==(const StringView& str) const;
        bool operator!=(const char* str) const;
        bool operator!=(const String& str) const;
        bool operator!=(const StringView& str) const;

        FORCE_INLINE char* operator*()
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE const char* operator*() const
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE char& operator[](uint64 index)
        {
            ASSERT(index < m_Length);
            return m_Data[index];
        }

        FORCE_INLINE char operator[](uint64 index) const
        {
            ASSERT(index < m_Length);
            return m_Data[index];
        }

    public:
        FORCE_INLINE char* Data()
        {
            return m_Data.Ptr();
        }

        FORCE_INLINE const char* Data() const
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

        FORCE_INLINE char& First()
        {
            return m_Data[0];
        }

        FORCE_INLINE const char& First() const
        {
            return m_Data[0];
        }

        FORCE_INLINE char& Last()
        {
            return m_Data[m_Length - 1];
        }

        FORCE_INLINE const char& Last() const
        {
            return m_Data[m_Length - 1];
        }

        FORCE_INLINE Iterator begin()
        {
            return Iterator(m_Data.Ptr());
        }

        FORCE_INLINE Iterator begin() const
        {
            return Iterator(const_cast<char*>(m_Data.Ptr()));
        }

        FORCE_INLINE Iterator end()
        {
            return Iterator(m_Data.Ptr() + m_Length);
        }

        FORCE_INLINE Iterator end() const
        {
            return Iterator(const_cast<char*>(m_Data.Ptr()) + m_Length);
        }

        FORCE_INLINE Iterator Begin()
        {
            return begin();
        }

        FORCE_INLINE Iterator Begin() const
        {
            return begin();
        }

        FORCE_INLINE Iterator End()
        {
            return end();
        }

        FORCE_INLINE Iterator End() const
        {
            return end();
        }

        FORCE_INLINE bool IsEmpty() const
        {
            return m_Length == 0 || m_Length == NO_POS || !m_Data.Ptr();
        }

    public:
        void Clear();
        void Free();
        void Resize(uint64 newLength);
        void Reserve(uint64 newCapacity, bool preserveContents = true);

        void ReCalcLength();
        void CorrectPathSlashes();
        void CorrectPlatformPathSlashes();

        void Set(const StringView& text);
        void Append(char ch);
        void Append(const StringView& text);

        void Concat(char left, const StringView& text);
        void Concat(char left, const String& text);
        void Concat(char left, const char* text);
        void Concat(char left, char ch);

        void Insert(uint64 index, const StringView& str);
        void Insert(uint64 index, char ch);
        void Remove(uint64 index, uint64 length);

        bool Compare(const StringView& str) const;

        String Substring(uint64 startIndex, uint64 length = NO_POS) const;
        String TrimTrailing() const;

        uint64 Find(const StringView& find, StringSearch opt = StringSearch::CaseSensitive, uint64 offset = 0) const;
        uint64 FindLast(const StringView& find, StringSearch opt = StringSearch::CaseSensitive, uint64 offset = 0) const;
        bool FindAll(const StringView& find, Array<uint64>& indices) const;

        bool BeginsWith(const StringView& text) const;
        bool EndsWith(const StringView& text) const;


        uint64 ReplaceChars(const char* search, uint64 searchLength, const char* replace, uint64 replaceLength, bool findFirst, StringSearch opt = StringSearch::CaseSensitive, uint64 offset = 0);
        void Replace(const StringView& find, const StringView& replace, StringSearch opt = StringSearch::CaseSensitive);
        void ReplaceFirst(const StringView& find, const StringView& replace, StringSearch opt = StringSearch::CaseSensitive);
        void ReplaceLast(const StringView& find, const StringView& replace, StringSearch opt = StringSearch::CaseSensitive);

    private:
        uint64 m_Length;
        Allocator m_Data;
    };

    Slice<char> ToSlice(const String& string);
    Slice<char> ToSlice(const String& string, uint64 index, uint64 length);
}
