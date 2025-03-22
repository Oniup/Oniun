#pragma once

#include "Oniun/Core/BaseTypes.h"

namespace Oniun
{
    enum class StringSearch
    {
        CaseSensitive,
        IgnoreCase,
    };

    namespace StringUtils
    {
        // Below up to utf conversions is from Godot but reformatted into the style of this codebase
        // https://github.com/godotengine/godot/blob/master/core/string/char_utils.h


        constexpr bool IsUpperCase(char ch)
        {
            return (ch >= L'A' && ch <= L'Z');
        }

        constexpr bool IsAsciiLowerCase(char ch)
        {
            return (ch >= L'a' && ch <= L'z');
        }

        constexpr bool IsDigit(char ch)
        {
            return (ch >= L'0' && ch <= L'9');
        }

        constexpr bool IsHexDigit(char ch)
        {
            return (IsDigit(ch) || (ch >= L'a' && ch <= L'f') || (ch >= L'A' && ch <= L'F'));
        }

        constexpr bool IsBinaryDigit(char ch)
        {
            return (ch == L'0' || ch == L'1');
        }

        constexpr bool IsAsciiAlphabetChar(char ch)
        {
            return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
        }

        constexpr bool IsAsciiAlphanumericChar(char ch)
        {
            return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9');
        }

        constexpr bool IsAsciiIdentifierChar(char ch)
        {
            return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9')
                || ch == L'_';
        }

        constexpr bool IsSymbol(char ch)
        {
            return ch != L'_' && ((ch >= L'!' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'['
                && ch <= L'`') || (ch >= L'{' && ch <= L'~') || ch == L'\t' || ch == L' ');
        }

        constexpr bool IsControl(char ch)
        {
            return (ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
        }

        constexpr bool IsWhitespace(char ch)
        {
            return (ch == L' ') || (ch == 0x00a0) || (ch == 0x1680) || (ch >= 0x2000 && ch <= 0x200a) ||
                (ch == 0x202f) || (ch == 0x205f) || (ch == 0x3000) || (ch == 0x2028) || (ch == 0x2029)
                || (ch >= 0x0009 && ch <= 0x000d) || (ch == 0x0085);
        }

        constexpr bool IsLinebreak(char ch)
        {
            return (ch >= 0x000a && ch <= 0x000d) || (ch == 0x0085) || (ch == 0x2028) || (ch == 0x2029);
        }

        constexpr bool IsPunctuation(char ch)
        {
            return (ch >= L' ' && ch <= L'/') || (ch >= L':' && ch <= L'@') || (ch >= L'[' && ch <= L'^')
                || (ch == L'`') || (ch >= L'{' && ch <= L'~') || (ch >= 0x2000 && ch <= 0x206f) || (ch
                    >= 0x3000 && ch <= 0x303f);
        }

        constexpr bool IsUnderscore(char ch)
        {
            return (ch == L'_');
        }

        constexpr char ToUpper(char ch)
        {
            if (ch >= 'a' && ch <= 'z')
                return ch - 32;
            return ch;
        }

        constexpr char ToLower(char ch)
        {
            if (ch >= 'A' && ch <= 'Z')
                return ch + 32;
            return ch;
        }

        // String Utf8ToUtf16(const CharStringView& str);
        // bool Utf16ToUtf8(const wchar_t* str, uint64 length, String& dest);

        constexpr uint64 Length(const char* str)
        {
            uint64 length = 0;
            while (true)
            {
                if (str[length] == L'\0')
                    return length;
                ++length;
            }
        }

        constexpr uint64 Find(const char* str, uint64 length, const char* search, uint64 searchLength, uint64 offset = 0)
        {
            if (searchLength > length)
                return NO_POS;

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
            return NO_POS;
        }

        constexpr uint64 FindIgnoreCase(const char* str, uint64 length, const char* search, uint64 searchLength,
                                        uint64 offset = 0)
        {
            if (searchLength > length)
                return NO_POS;

            for (uint64 i = offset; i < length - searchLength + 1; ++i)
            {
                bool found = true;
                for (uint64 j = 0; j < searchLength; ++j)
                {
                    char ch = ToUpper(str[i + j]);
                    char sCh = ToUpper(search[j]);
                    if (ch != sCh)
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                    return i;
            }
            return NO_POS;
        }

        constexpr uint64 Find(const char* str, uint64 length, char search, uint64 offset = 0)
        {
            for (uint64 i = offset; i < length; ++i)
            {
                if (str[i] == search)
                    return i;
            }
            return NO_POS;
        }

        constexpr uint64 FindLast(const char* str, uint64 length, const char* search, uint64 searchLength,
                                  uint64 offset = 0)
        {
            if (searchLength > length)
                return NO_POS;

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
            return NO_POS;
        }

        constexpr uint64 FindLastIgnoreCase(const char* str, uint64 length, const char* search, uint64 searchLength,
                                            uint64 offset = 0)
        {
            if (searchLength > length)
                return NO_POS;

            for (uint64 i = length - searchLength - offset; i > 0; --i)
            {
                bool found = true;
                for (uint64 j = 0; j < searchLength; ++j)
                {
                    char ch = ToUpper(str[i + j]);
                    char sCh = ToUpper(search[j]);
                    if (ch != sCh)
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                    return i;
            }
            return NO_POS;
        }

        constexpr uint64 FindLast(const char* str, uint64 length, char search, uint64 offset = 0)
        {
            for (uint64 i = length - offset; i > 0; --i)
            {
                if (str[i - 1] == search)
                    return i - 1;
            }
            return NO_POS;
        }


        constexpr bool BeginsWith(const char* str, uint64 length, const char* search, uint64 searchLength)
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

        constexpr bool EndsWith(const char* str, uint64 length, const char* search, uint64 searchLength)
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
}
