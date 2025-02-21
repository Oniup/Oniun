#include "Oniun/Core/String/StringUtils.h"

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"

namespace Onu::StringUtils
{
    String Utf8ToUtf16(const CharStringView& str)
    {
        // https://stackoverflow.com/questions/7153935/how-to-convert-utf-8-stdstring-to-utf-16-stdwstring
        // Parse utf-8 string and convert into unicode array
        Array<uint32> unicode;
        uint64 utf16Length;
        if (!UftConversion_Internal::Utf8ToUnicode(unicode, str, utf16Length))
            return String::Empty;

        // Convert utf-32 into utf16
        String utf16(utf16Length);
        for (uint64 i = 0; i < unicode.Count(); ++i)
        {
            unsigned long uni = unicode[i];
            if (uni <= 0xFFFF)
            {
                utf16 += (Char)uni;
            }
            else
            {
                uni -= 0x10000;
                utf16 += (Char)((uni >> 10) + 0xD800);
                utf16 += (Char)((uni & 0x3FF) + 0xDC00);
            }
        }
        return utf16;
    }

    CharString Utf16ToUtf8(const StringView& str)
    {
        Array<uint32> unicode;
        uint64 utf8Length;
        if (!UftConversion_Internal::Utf16ToUnicode(unicode, str, utf8Length))
            return CharString::Empty;

        CharString result(utf8Length);
        // CharString result;
        for (int32 j = 0; j < unicode.Count(); j++)
        {
            uint32 uni = unicode[j];
            uint32 count = 0;
            if (uni <= 0x7FU)
                count = 1;
            else if (uni <= 0x7FFU)
                count = 2;
            else if (uni <= 0xFFFFU)
                count = 3;
            else if (uni <= 0x1FFFFFU)
                count = 4;
            else if (uni <= 0x3FFFFFFU)
                count = 5;
            else if (uni <= 0x7FFFFFFFU)
                count = 6;

            result += (char)(count <= 1 ? (uint8)uni : ((uint8(0xFFU) << (8 - count)) | uint8(uni >> (6 * (count - 1)))));
            for (uint32 k = 1; k < count; k++)
                result += char(uint8(0x80U | (uint8(0x3FU) & uint8(uni >> (6 * (count - 1 - k))))));
        }
        return result;
    }

    bool UftConversion_Internal::Utf8ToUnicode(Array<uint32>& unicode, const CharStringView& str, uint64& utf16Length)
    {
        unicode.Reserve(str.Length(), false);
        utf16Length = 0;
        uint64 i = 0;
        while (i < str.Length())
        {
            uint32 uni;
            uint64 todo;
            uint8 ch = str[i++];
            if (ch <= 0x7F)
            {
                uni = ch;
                todo = 0;
            }
            else if (ch <= 0xBF)
            {
                fprintf(stderr, "'%s' not a UTF-8 string", *str);
                return false;
            }
            else if (ch <= 0xDF)
            {
                uni = ch & 0x1F;
                todo = 1;
            }
            else if (ch <= 0xEF)
            {
                uni = ch & 0x0F;
                todo = 2;
            }
            else if (ch <= 0xF7)
            {
                uni = ch & 0x07;
                todo = 3;
            }
            else
            {
                fprintf(stderr, "'%s' not a UTF-8 string", *str);
                return false;
            }

            for (size_t j = 0; j < todo; ++j)
            {
                if (i == str.Length())
                {
                    fprintf(stderr, "'%s' not a UTF-8 string", *str);
                    return false;
                }
                uint8 c = str[i++];
                if (c < 0x80 || c > 0xBF)
                {
                    fprintf(stderr, "'%s' not a UTF-8 string", *str);
                    return false;
                }
                uni <<= 6;
                uni += c & 0x3F;
            }

            if (uni >= 0xD800 && uni <= 0xDFFF)
            {
                fprintf(stderr, "'%s' not a UTF-8 string", *str);
                return false;
            }
            if (uni > 0x10FFFF)
            {
                fprintf(stderr, "'%s' not a UTF-8 string", *str);
                return false;
            }
            unicode.Add(uni);

            ++utf16Length;
            if (uni > 0xFFFF)
                ++utf16Length;
        }
        return true;
    }

    bool UftConversion_Internal::Utf16ToUnicode(Array<uint32>& unicode, const StringView& str, uint64& utf8Length)
    {
        unicode.Reserve(str.Length(), false);
        utf8Length = 0;
        int32 i = 0;
        while (i < str.Length())
        {
            uint32 uni = str[i++];
            if (uni < 0xD800U || uni > 0xDFFFU)
            {
            }
            else if (uni >= 0xDC00U)
            {
                fwprintf(stderr, TEXT("'%s' not a UTF-16 string"), *str);
                return false;
            }
            else if (i + 1 == str.Length())
            {
                fwprintf(stderr, TEXT("'%s' not a UTF-16 string"), *str);
                return false;
            }
            else if (i < str.Length())
            {
                uni = (uni & 0x3FFU) << 10;
                if ((str[i] < 0xDC00U) || (str[i] > 0xDFFFU))
                {
                fwprintf(stderr, TEXT("'%s' not a UTF-16 string"), *str);
                return false;
                }
                uni |= str[i++] & 0x3FFU;
                uni += 0x10000U;
            }

            unicode.Add(uni);

            if (uni <= 0x7FU)
                utf8Length += 1;
            else if (uni <= 0x7FFU)
                utf8Length += 2;
            else if (uni <= 0xFFFFU)
                utf8Length += 3;
            else if (uni <= 0x1FFFFFU)
                utf8Length += 4;
            else if (uni <= 0x3FFFFFFU)
                utf8Length += 5;
            else if (uni <= 0x7FFFFFFFU)
                utf8Length += 6;
        }
        return true;
    }
}
