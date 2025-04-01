#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    FormatArgsContext::ArgIterator::ArgIterator()
        : m_Context(nullptr), m_CurrentBegin(0), m_CurrentEnd(0)
    {
    }

    FormatArgsContext::ArgIterator::ArgIterator(FormatArgsContext* context, uint64 begin, uint64 end)
        : m_Context(context), m_CurrentBegin(begin), m_CurrentEnd(end)
    {
    }

    FormatArgsContext::ArgIterator::ArgIterator(const ArgIterator& iter)
        : m_Context(iter.m_Context), m_CurrentBegin(iter.m_CurrentBegin), m_CurrentEnd(iter.m_CurrentEnd)
    {
    }

    FormatArgsContext::ArgIterator::ArgIterator(ArgIterator&& iter)
        : m_Context(iter.m_Context), m_CurrentBegin(iter.m_CurrentBegin),
          m_CurrentEnd(iter.m_CurrentEnd)
    {
        iter.m_Context = nullptr;
        iter.m_CurrentBegin = 0;
        iter.m_CurrentEnd = 0;
    }

    FormatArgsContext::ArgIterator& FormatArgsContext::ArgIterator::operator=(const ArgIterator& iter)
    {
        m_Context = iter.m_Context;
        m_CurrentBegin = iter.m_CurrentBegin;
        m_CurrentEnd = iter.m_CurrentEnd;
        return *this;
    }

    FormatArgsContext::ArgIterator& FormatArgsContext::ArgIterator::operator=(ArgIterator&& iter)
    {
        m_Context = iter.m_Context;
        m_CurrentBegin = iter.m_CurrentBegin;
        m_CurrentEnd = iter.m_CurrentEnd;

        iter.m_Context = nullptr;
        iter.m_CurrentBegin = 0;
        iter.m_CurrentEnd = 0;
        return *this;
    }

    StringView FormatArgsContext::ArgIterator::operator*() const
    {
        return Get();
    }

    bool FormatArgsContext::ArgIterator::operator==(const ArgIterator& iter) const
    {
        return m_Context == iter.m_Context && m_CurrentBegin == iter.m_CurrentBegin && iter.m_CurrentEnd ==
            m_CurrentEnd;
    }

    bool FormatArgsContext::ArgIterator::operator!=(const ArgIterator& iter) const
    {
        return !(*this == iter);
    }

    FormatArgsContext::ArgIterator& FormatArgsContext::ArgIterator::operator++()
    {
        ++m_CurrentEnd;
        m_CurrentBegin = m_CurrentEnd;
        for (uint64 i = m_CurrentBegin; i < m_Context->m_Args.Length() - 1; ++i)
        {
            if (m_Context->m_Args[i] == FORMAT_ARG_SEPARATOR_SYNTAX)
                break;
            ++m_CurrentEnd;
        }
        if (m_CurrentBegin > m_Context->m_Args.Length())
        {
            m_CurrentBegin = NO_POS;
            m_CurrentEnd = NO_POS;
        }
        return *this;
    }

    FormatArgsContext::FormatArgsContext(const StringView& options)
        : m_Args(options)
    {
        // Check if insert options are valid by checking correct format
        if (m_Args.Length() < 2 || m_Args.First() != '{' || m_Args.Last() != '}')
            m_Args = nullptr;
    }

#define PRIMITIVE_FORMATTER_CLASS_FLOAT(_Type) \
    bool Formatter<_Type>::Parse(const FormatArgsContext& context) \
    { \
        for (StringView arg : context) \
        { \
            if (arg.First() == '.') \
                Precision = arg.Last() - '0'; \
            else if (arg == "e" || arg == "E") \
                ScientificNotation = true; \
        } \
        return true; \
    } \
    void Formatter<_Type>::FormatTo(String& dest, _Type value) \
    { \
        constexpr uint64 cFormatMaxLength = 8; \
        char cFormat[cFormatMaxLength]; \
        if (Precision != NO_POS) \
            Crt::Format(cFormat, cFormatMaxLength, "%%.%lluf", Precision); \
        else if (ScientificNotation) \
            Crt::Copy(cFormat, "%e\0", 3); \
        else \
            Crt::Copy(cFormat, "%f\0", 3); \
        constexpr uint64 bufferMaxLength = 20; \
        char buffer[bufferMaxLength]; \
        Crt::Format(buffer, bufferMaxLength, cFormat, value); \
        dest.Append(buffer); \
    }

#define PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(_Bits, _Format) \
    bool Formatter<uint##_Bits>::Parse(const FormatArgsContext& context) \
    { \
        for (StringView arg : context) \
        { \
            if (arg == "x" || arg == "X") \
                Hexadecimal = true; \
            if (arg == "p") \
                Pointer = true; \
        } \
        return true; \
    } \
    void Formatter<uint##_Bits>::FormatTo(String& dest, uint##_Bits value) \
    { \
        constexpr uint##_Bits bufferMaxLength = 20; \
        char buffer[bufferMaxLength]; \
        if (Hexadecimal) \
            Crt::Format(buffer, bufferMaxLength, "%x", value); \
        else if (Pointer) \
            Crt::Format(buffer, bufferMaxLength, "%p", value); \
        else \
            Crt::Format(buffer, bufferMaxLength, _Format, value); \
        dest.Append(buffer); \
    }

#define PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(_Bits, _Format) \
    bool Formatter<int##_Bits>::Parse(const FormatArgsContext& context) \
    { \
        for (StringView arg : context) \
        { \
            if (arg == "p") \
                Pointer = true; \
        } \
        return true; \
    } \
    void Formatter<int##_Bits>::FormatTo(String& dest, int##_Bits value) \
    { \
        constexpr int##_Bits bufferMaxLength = 20; \
        char buffer[bufferMaxLength]; \
        if (Pointer) \
            Crt::Format(buffer, bufferMaxLength, "%p", value); \
        else \
            Crt::Format(buffer, bufferMaxLength, _Format, value); \
        dest.Append(buffer); \
    }

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

#define FORMATTER_CLASS_STRING(_Type) \
    bool Formatter<_Type>::Parse(const FormatArgsContext& context) \
    { \
        return true; \
    } \
    void Formatter<_Type>::FormatTo(String& dest, const _Type& str) \
    { \
        dest.Append(str); \
    }

#define PRIMITIVE_STRING_TYPE_FORMATTER_CLASS(_Type) \
    bool Formatter<_Type>::Parse(const FormatArgsContext& context) \
    { \
        return true; \
    } \
    void Formatter<_Type>::FormatTo(String& dest, const char* str) \
    { \
        dest.Append(str); \
    }

    FORMATTER_CLASS_STRING(String)
    FORMATTER_CLASS_STRING(StringView)
    PRIMITIVE_STRING_TYPE_FORMATTER_CLASS(char*)
    PRIMITIVE_STRING_TYPE_FORMATTER_CLASS(const char*)
}
