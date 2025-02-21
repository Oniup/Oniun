#pragma once

#include "Oniun/Core/BaseTypes.h"

namespace Onu::StringUtils
{
    String Utf8ToUtf16(const CharStringView& str);
    CharString Utf16ToUtf8(const StringView& str);

    namespace UftConversion_Internal
    {
        bool Utf8ToUnicode(Array<uint32>& unicode, const CharStringView& str, uint64& utf16Length);
        bool Utf16ToUnicode(Array<uint32>& unicode, const StringView& str, uint64& utf8Length);
    }

    template<typename TChar>
    static constexpr uint64 Length(const TChar* str)
    {
        uint64 length = 0;
        while (true)
        {
            if (str[length] == L'\0')
                return length;
            ++length;
        }
    }

    // Below is from Godot but reformatted into the style of this codebase
    // https://github.com/godotengine/godot/blob/master/core/string/char_utils.h

    template<typename TChar>
    static constexpr bool IsUpperCase(TChar ch)
    {
        return (ch >= L'A' && ch <= L'Z');
    }

    template<typename TChar>
    static constexpr bool IsAsciiLowerCase(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z');
    }

    template<typename TChar>
    static constexpr bool IsDigit(TChar ch)
    {
        return (ch >= L'0' && ch <= L'9');
    }

    template<typename TChar>
    static constexpr bool IsHexDigit(TChar ch)
    {
        return (IsDigit(ch) || (ch >= L'a' && ch <= L'f') || (ch >= L'A' && ch <= L'F'));
    }

    template<typename TChar>
    static constexpr bool IsBinaryDigit(TChar ch)
    {
        return (ch == L'0' || ch == L'1');
    }

    template<typename TChar>
    static constexpr bool IsAsciiAlphabetChar(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
    }

    template<typename TChar>
    static constexpr bool IsAsciiAlphanumericChar(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9');
    }

    template<typename TChar>
    static constexpr bool IsAsciiIdentifierChar(TChar ch)
    {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9')
            || ch == L'_';
    }

    template<typename TChar>
    static constexpr bool IsSymbol(TChar ch)
    {
        return ch != L'_' && ((ch >= L'!' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'['
            && ch <= L'`') || (ch >= L'{' && ch <= L'~') || ch == L'\t' || ch == L' ');
    }

    template<typename TChar>
    static constexpr bool IsControl(TChar ch)
    {
        return (ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
    }

    template<typename TChar>
    static constexpr bool IsWhitespace(TChar ch)
    {
        return (ch == L' ') || (ch == 0x00a0) || (ch == 0x1680) || (ch >= 0x2000 && ch <= 0x200a) ||
            (ch == 0x202f) || (ch == 0x205f) || (ch == 0x3000) || (ch == 0x2028) || (ch == 0x2029)
            || (ch >= 0x0009 && ch <= 0x000d) || (ch == 0x0085);
    }

    template<typename TChar>
    static constexpr bool IsLinebreak(TChar ch)
    {
        return (ch >= 0x000a && ch <= 0x000d) || (ch == 0x0085) || (ch == 0x2028) || (ch == 0x2029);
    }

    template<typename TChar>
    static constexpr bool IsPunctuation(TChar ch)
    {
        return (ch >= L' ' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'[' && ch <= L'^')
            || (ch == L'`') || (ch >= L'{' && ch <= L'~') || (ch >= 0x2000 && ch <= 0x206f) || (ch
                >= 0x3000 && ch <= 0x303f);
    }

    template<typename TChar>
    static constexpr bool IsUnderscore(TChar ch)
    {
        return (ch == L'_');
    }
}
