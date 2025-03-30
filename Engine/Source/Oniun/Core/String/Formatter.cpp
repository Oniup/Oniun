#include "Oniun/Core/String/Formatter.h"

namespace Oniun
{
    FormatParseArgsContext::FormatParseArgsContext(const StringView& options)
        : m_Args(options), m_CurrentBegin(0), m_CurrentEnd(0)
    {
        // Check if insert options are valid by checking correct format
        if (m_Args.Length() < 2 || m_Args.First() != '{' || m_Args.Last() != '}')
            m_Args = nullptr;
    }

    bool FormatParseArgsContext::Next()
    {
        ++m_CurrentEnd;
        m_CurrentBegin = m_CurrentEnd;
        for (uint64 i = m_CurrentBegin; i < m_Args.Length() - 1; ++i)
        {
            if (m_Args[i] == FORMAT_ARG_SEPARATOR_SYNTAX)
                break;
            ++m_CurrentEnd;
        }
        return m_CurrentBegin < m_Args.Length() - 1;
    }

#define PRIMITIVE_FORMATTER_CLASS_FLOAT(_Type) \
    Formatter<_Type>::Formatter() \
        : m_ScientificNotation(false), m_Precision(NO_POS) \
    { \
    } \
    bool Formatter<_Type>::Parse(FormatParseArgsContext& context) \
    { \
        while (context.Next()) \
        { \
            StringView arg(context.Get()); \
            if (arg.First() == '.') \
                m_Precision = arg.Last() - '0'; \
            else if (arg == "e" || arg == "E") \
                m_ScientificNotation = true; \
        } \
        return true; \
    } \
    void Formatter<_Type>::FormatTo(String& dest, _Type value) \
    { \
        constexpr uint64 cFormatMaxLength = 8; \
        char cFormat[cFormatMaxLength]; \
        if (m_Precision != NO_POS) \
            Crt::Format(cFormat, cFormatMaxLength, "%%.%lluf", m_Precision); \
        else if (m_ScientificNotation) \
            Crt::Copy(cFormat, "%e\0", 3); \
        else \
            Crt::Copy(cFormat, "%f\0", 3); \
        constexpr uint64 bufferMaxLength = 20; \
        char buffer[bufferMaxLength]; \
        Crt::Format(buffer, bufferMaxLength, cFormat, value); \
        dest.Append(buffer); \
    } \

#define PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(_Bits, _Format) \
    Formatter<uint##_Bits>::Formatter() \
        : m_Hexadecimal(false), m_Pointer(false) \
    { \
    } \
    bool Formatter<uint##_Bits>::Parse(FormatParseArgsContext& context) \
    { \
        while (context.Next()) \
        { \
            StringView arg(context.Get()); \
            if (arg == "x" || arg == "X") \
                m_Hexadecimal = true; \
            if (arg == "p") \
                m_Pointer = true; \
        } \
        return true; \
    } \
    void Formatter<uint##_Bits>::FormatTo(String& dest, uint##_Bits value) \
    { \
        constexpr uint##_Bits bufferMaxLength = 20; \
        char buffer[bufferMaxLength]; \
        if (m_Hexadecimal) \
            Crt::Format(buffer, bufferMaxLength, "%x", value); \
        else if (m_Pointer) \
            Crt::Format(buffer, bufferMaxLength, "%p", value); \
        else \
            Crt::Format(buffer, bufferMaxLength, _Format, value); \
        dest.Append(buffer); \
    } \

#define PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(_Bits, _Format) \
    Formatter<int##_Bits>::Formatter() \
        : m_Pointer(false) \
    { \
    } \
    bool Formatter<int##_Bits>::Parse(FormatParseArgsContext& context) \
    { \
        while (context.Next()) \
        { \
            StringView arg(context.Get()); \
            if (arg == "p") \
                m_Pointer = true; \
        } \
        return true; \
    } \
    void Formatter<int##_Bits>::FormatTo(String& dest, int##_Bits value) \
    { \
        constexpr int##_Bits bufferMaxLength = 20; \
        char buffer[bufferMaxLength]; \
        if (m_Pointer) \
            Crt::Format(buffer, bufferMaxLength, "%p", value); \
        else \
            Crt::Format(buffer, bufferMaxLength, _Format, value); \
        dest.Append(buffer); \
    } \

    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(8, "%u")
    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(16, "%u")
    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(32, "%u")
    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(64, "%llu")

    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(8, "%d")
    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(16, "%d")
    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(32, "%d")
    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(64, "%lld")

    PRIMITIVE_FORMATTER_CLASS_FLOAT(float)
    PRIMITIVE_FORMATTER_CLASS_FLOAT(double)

    //Formatter<uint64>::Formatter() : m_Hexadecimal(false), m_Pointer(false)
    //{
    //}

    //bool Formatter<uint64>::Parse(FormatParseArgsContext& context)
    //{
    //    while (context.Next())
    //    {
    //        StringView arg(context.Get());
    //        if (arg == "x" || arg == "X")
    //            m_Hexadecimal = true;
    //        if (arg == "p")
    //            m_Pointer = true;
    //    } return true;
    //}

    //void Formatter<uint64>::FormatTo(String& dest, uint64 value)
    //{
    //    constexpr uint64 bufferMaxLength = 20;
    //    char buffer[bufferMaxLength];
    //    if (m_Hexadecimal)
    //        Crt::Format(buffer, bufferMaxLength, "%x", value);
    //    else if (m_Pointer)
    //        Crt::Format(buffer, bufferMaxLength, "%p", value);
    //    else
    //        Crt::Format(buffer, bufferMaxLength, "%llu", value); dest.Append(buffer);
    //}
}

