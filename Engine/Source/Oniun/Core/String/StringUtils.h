#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/GlobalVars.h"

enum class StringSearch
{
    CaseSensitive,
    IgnoreCase,
};

namespace StringUtils
{
    // Below up to utf conversions is from Godot but reformatted into the style of this codebase
    // https://github.com/godotengine/godot/blob/master/core/string/char_utils.h

    template<typename TChar>
    constexpr bool IsUpperCase(TChar ch)
    {
        return (ch >= L'A' && ch <= L'Z');
    }

    template<typename TChar>
    constexpr bool IsAsciiLowerCase(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z');
    }

    template<typename TChar>
    constexpr bool IsDigit(TChar ch)
    {
        return (ch >= L'0' && ch <= L'9');
    }

    template<typename TChar>
    constexpr bool IsHexDigit(TChar ch)
    {
        return (IsDigit(ch) || (ch >= L'a' && ch <= L'f') || (ch >= L'A' && ch <= L'F'));
    }

    template<typename TChar>
    constexpr bool IsBinaryDigit(TChar ch)
    {
        return (ch == L'0' || ch == L'1');
    }

    template<typename TChar>
    constexpr bool IsAsciiAlphabetChar(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
    }

    template<typename TChar>
    constexpr bool IsAsciiAlphanumericChar(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9');
    }

    template<typename TChar>
    constexpr bool IsAsciiIdentifierChar(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9')
            || ch == L'_';
    }

    template<typename TChar>
    constexpr bool IsSymbol(TChar ch)
    {
        return ch != L'_' && ((ch >= L'!' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'['
            && ch <= L'`') || (ch >= L'{' && ch <= L'~') || ch == L'\t' || ch == L' ');
    }

    template<typename TChar>
    constexpr bool IsControl(TChar ch)
    {
        return (ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
    }

    template<typename TChar>
    constexpr bool IsWhitespace(TChar ch)
    {
        return (ch == L' ') || (ch == 0x00a0) || (ch == 0x1680) || (ch >= 0x2000 && ch <= 0x200a) ||
            (ch == 0x202f) || (ch == 0x205f) || (ch == 0x3000) || (ch == 0x2028) || (ch == 0x2029)
            || (ch >= 0x0009 && ch <= 0x000d) || (ch == 0x0085);
    }

    template<typename TChar>
    constexpr bool IsLinebreak(TChar ch)
    {
        return (ch >= 0x000a && ch <= 0x000d) || (ch == 0x0085) || (ch == 0x2028) || (ch == 0x2029);
    }

    template<typename TChar>
    constexpr bool IsPunctuation(TChar ch)
    {
        return (ch >= L' ' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'[' && ch <= L'^')
            || (ch == L'`') || (ch >= L'{' && ch <= L'~') || (ch >= 0x2000 && ch <= 0x206f) || (ch
                >= 0x3000 && ch <= 0x303f);
    }

    template<typename TChar>
    constexpr bool IsUnderscore(TChar ch)
    {
        return (ch == L'_');
    }

    template<typename TChar>
    constexpr TChar ToUpper(TChar ch)
    {
        if (ch >= 'a' && ch <= 'z')
            return ch - 32;
        return ch;
    }

    template<typename TChar>
    constexpr TChar ToLower(TChar ch)
    {
        if (ch >= 'A' && ch <= 'Z')
            return ch + 32;
        return ch;
    }

    String Utf8ToUtf16(const CharStringView& str);
    CharString Utf16ToUtf8(const StringView& str);

    namespace UftConversion_Internal
    {
        bool Utf8ToUnicode(Array<uint32>& unicode, const CharStringView& str, uint64& utf16Length);
        bool Utf16ToUnicode(Array<uint32>& unicode, const StringView& str, uint64& utf8Length);
    }

    template<typename TChar>
    constexpr uint64 Length(const TChar* str)
    {
        uint64 length = 0;
        while (true)
        {
            if (str[length] == L'\0')
                return length;
            ++length;
        }
    }

    template<typename TChar>
    constexpr uint64 Find(const TChar* str, uint64 length, const TChar* search, uint64 searchLength, uint64 offset = 0)
    {
        if (searchLength > length)
            return GlobalVars::NoPos;

        for (uint64 i = offset; i < length - searchLength + 1; ++i)
        {
            bool found = true;
            for (uint64 j = 0; j < searchLength; ++j)
            {
                if (str[i + j] != search[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return GlobalVars::NoPos;
    }

    template<typename TChar>
    constexpr uint64 FindIgnoreCase(const TChar* str, uint64 length, const TChar* search, uint64 searchLength, uint64 offset = 0)
    {
        if (searchLength > length)
            return GlobalVars::NoPos;

        for (uint64 i = offset; i < length - searchLength + 1; ++i)
        {
            bool found = true;
            for (uint64 j = 0; j < searchLength; ++j)
            {
                TChar ch = ToUpper(str[i + j]);
                TChar sCh = ToUpper(search[j]);
                if (ch != sCh)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return GlobalVars::NoPos;
    }

    template<typename TChar>
    constexpr uint64 Find(const TChar* str, uint64 length, TChar search, uint64 offset = 0)
    {
        for (uint64 i = offset; i < length; ++i)
        {
            if (str[i] == search)
                return i;
        }
        return GlobalVars::NoPos;
    }

    template<typename TChar>
    constexpr uint64 FindLast(const TChar* str, uint64 length, const TChar* search, uint64 searchLength,  uint64 offset = 0)
    {
        if (searchLength > length)
            return GlobalVars::NoPos;

        for (uint64 i = length - searchLength - offset; i > 0; --i)
        {
            bool found = true;
            for (uint64 j = 0; j < searchLength; ++j)
            {
                if (str[i + j] != search[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return GlobalVars::NoPos;
    }

    template<typename TChar>
    constexpr uint64 FindLastIgnoreCase(const TChar* str, uint64 length, const TChar* search, uint64 searchLength,  uint64 offset = 0)
    {
        if (searchLength > length)
            return GlobalVars::NoPos;

        for (uint64 i = length - searchLength - offset; i > 0; --i)
        {
            bool found = true;
            for (uint64 j = 0; j < searchLength; ++j)
            {
                TChar ch = ToUpper(str[i + j]);
                TChar sCh = ToUpper(search[j]);
                if (ch != sCh)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return GlobalVars::NoPos;
    }

    template<typename TChar>
    constexpr uint64 FindLast(const TChar* str, uint64 length, TChar search, uint64 offset = 0)
    {
        for (uint64 i = length - offset; i > 0; --i)
        {
            if (str[i - 1] == search)
                return i - 1;
        }
        return GlobalVars::NoPos;
    }

    template<typename TChar>
    constexpr bool BeginsWith(const TChar* str, uint64 length, const TChar* search, uint64 searchLength)
    {
        if (searchLength > length)
            return false;
        for (uint64 i = 0; i < searchLength; ++i)
        {
            if (str[i] != search[i])
                return false;
        }
        return true;
    }

    template<typename TChar>
    constexpr bool EndsWith(const TChar* str, uint64 length, const TChar* search, uint64 searchLength)
    {
        if (searchLength > length)
            return false;
        uint64 offset = length - searchLength;
        for (uint64 i = 0; i < searchLength; ++i)
        {
            if (str[offset + i] != search[i])
                return false;
        }
        return true;
    }
}
