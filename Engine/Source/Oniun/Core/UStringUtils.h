#pragma once

#include "Oniun/Core/BaseTypes.h"

namespace Onu::UStringUtils
{
    constexpr uint64 Length(const Char* text)
    {
        uint64 length = 0;
        while (true)
        {
            if (text[length] == L'\0')
                return length;
            ++length;
        }
    }

	// Below is from Godot but reformatted into the style of this codebase
	// https://github.com/godotengine/godot/blob/master/core/string/char_utils.h

    constexpr bool IsUpperCase(Char ch)
    {
	    return (ch >= L'A' && ch <= L'Z');
    }

    constexpr bool IsAsciiLowerCase(Char ch)
    {
	    return (ch >= L'a' && ch <= L'z');
    }

    constexpr bool IsDigit(Char ch)
    {
	    return (ch >= L'0' && ch <= L'9');
    }

    constexpr bool IsHexDigit(Char ch)
    {
	    return (IsDigit(ch) || (ch >= L'a' && ch <= L'f') || (ch >= L'A' && ch <= L'F'));
    }

    constexpr bool IsBinaryDigit(Char ch)
    {
	    return (ch == L'0' || ch == L'1');
    }

    constexpr bool IsAsciiAlphabetChar(Char ch)
    {
	    return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
    }

    constexpr bool IsAsciiAlphanumericChar(Char ch)
    {
	    return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9');
    }

    constexpr bool IsAsciiIdentifierChar(Char ch)
    {
	    return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9')
		    || ch == L'_';
    }

    constexpr bool IsSymbol(Char ch)
    {
	    return ch != L'_' && ((ch >= L'!' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'['
		    && ch <= L'`') || (ch >= L'{' && ch <= L'~') || ch == L'\t' || ch == L' ');
    }

    constexpr bool IsControl(Char ch)
    {
	    return (ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
    }

    constexpr bool IsWhitespace(Char ch)
    {
	    return (ch == L' ') || (ch == 0x00a0) || (ch == 0x1680) || (ch >= 0x2000 && ch <= 0x200a) ||
		    (ch == 0x202f) || (ch == 0x205f) || (ch == 0x3000) || (ch == 0x2028) || (ch == 0x2029)
		    || (ch >= 0x0009 && ch <= 0x000d) || (ch == 0x0085);
    }

    constexpr bool IsLinebreak(Char ch)
    {
	    return (ch >= 0x000a && ch <= 0x000d) || (ch == 0x0085) || (ch == 0x2028) || (ch == 0x2029);
    }

    constexpr bool IsPunctuation(Char ch)
    {
	    return (ch >= L' ' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'[' && ch <= L'^')
		    || (ch == L'`') || (ch >= L'{' && ch <= L'~') || (ch >= 0x2000 && ch <= 0x206f) || (ch
			    >= 0x3000 && ch <= 0x303f);
    }

    constexpr bool IsUnderscore(Char ch)
    {
	    return (ch == L'_');
    }
}
