#pragma once

#include "Oniun/Core/String/String.h"

// TODO: Replace Next with an iterator
// TODO: Add String types, HashMap, Pair, DateTime, LinkList, Slice, Function

namespace Oniun
{
#define FORMAT_ARG_SEPARATOR_SYNTAX '|'
#define FORMAT_ARG_CONTAINER_OPT_SYNTAX ':'

    class FormatParseArgsContext
    {
    public:
        FormatParseArgsContext(const StringView& options);

    public:
        FORCE_INLINE StringView Get()
        {
            return StringView(m_Args.Data() + m_CurrentBegin, m_CurrentEnd - m_CurrentBegin);
        }

        FORCE_INLINE bool IsValid()
        {
            return !m_Args.IsEmpty();
        }

        FORCE_INLINE void Reset()
        {
            m_CurrentBegin = 0;
            m_CurrentEnd = 0;
        }

    public:
        bool Next();

    private:
        StringView m_Args;
        uint64 m_CurrentBegin;
        uint64 m_CurrentEnd;
    };

    template <typename T>
    class Formatter
    {
    public:
        /// @brief Here you parse the arguments passed within the {} and store the options within the formatter to be
        ///        used when FormatTo(...) is called.
        /// @note  If you don't need to parse any options just return true.
        /// 
        /// @param context Format option arguments for formatting the type.
        /// @return True if should continue to format into a string, otherwise parse failed and should append into
        ///         destination string.
        bool Parse(FormatParseArgsContext& context) = delete;

        /// @brief Format and append the resulting string to the destination string.
        /// 
        /// @param dest  Destinationn string to push string formatted verison of type to.
        /// @param value Data of target type to stringify
        void FormatTo(String& dest, const T& value) = delete;
    };

#define PRIMITIVE_FORMATTER_CLASS_FLOAT(_Type) \
    template <> \
    class Formatter<_Type> \
    { \
    public: \
        Formatter(); \
        bool Parse(FormatParseArgsContext& context); \
        void FormatTo(String& dest, _Type value); \
    private: \
        bool m_ScientificNotation; \
        uint64 m_Precision; \
    }; \

#define PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(_Bits) \
    template <> \
    class Formatter<int##_Bits> \
    { \
    public: \
        Formatter(); \
        bool Parse(FormatParseArgsContext& context); \
        void FormatTo(String& dest, int##_Bits value); \
    private: \
        bool m_Pointer; \
    }; \

#define PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(_Bits) \
    template <> \
    class Formatter<uint##_Bits> \
    { \
    public: \
        Formatter(); \
        bool Parse(FormatParseArgsContext& context); \
        void FormatTo(String& dest, uint##_Bits value); \
    private: \
        bool m_Hexadecimal; \
        bool m_Pointer; \
    }; \

    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(8)
    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(16)
    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(32)
    PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(64)

    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(8)
    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(16)
    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(32)
    PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(64)

    PRIMITIVE_FORMATTER_CLASS_FLOAT(float)
    PRIMITIVE_FORMATTER_CLASS_FLOAT(double)

#undef PRIMITIVE_FORMATTER_CLASS_SIGNED_INT
#undef PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT
#undef PRIMITIVE_FORMATTER_CLASS_FLOAT

    template <typename T, typename TAllocationType>
    class Formatter<Array<T, TAllocationType>>
    {
    private:
        static constexpr const char* DefaultSeparator = ",";

    public:
        Formatter()
            : m_NewLinePerElement(false), m_Compact(false), m_RemoveSeparator(false), m_RemoveBrackets(false),
              m_Numbered(false), m_Separator(',')
        {
        }

        bool Parse(FormatParseArgsContext& context)
        {
            bool isArrOptions = true;
            while (context.Next())
            {
                StringView arg(context.Get());
                if (isArrOptions)
                {
                    if (arg == "ln")
                        m_NewLinePerElement = true;
                    else if (arg == "cp")
                        m_Compact = true;
                    else if (arg == "rs")
                        m_RemoveSeparator = true;
                    else if (arg == "rb")
                        m_RemoveBrackets = true;
                    else if (arg == "num")
                        m_Numbered = true;
                    else if (arg.Find("s:") != NO_POS)
                        m_Separator = arg.Last();
                    else if (arg == "elem")
                        isArrOptions = false;
                }
                else
                {
                    bool first = m_ElementOptions.IsEmpty();
                    if (first)
                        m_ElementOptions.Append('{');
                    else 
                        m_ElementOptions.Append(FORMAT_ARG_SEPARATOR_SYNTAX);
                    m_ElementOptions.Append(arg);
                }
            }
            if (!m_ElementOptions.IsEmpty())
                m_ElementOptions.Append('}');
            return true;
        }

        void FormatTo(String& dest, const Array<T, TAllocationType>& array)
        {
            if (!m_RemoveBrackets)
                dest.Append('{');

            Formatter<T> elemFormatter;
            FormatParseArgsContext elemContext(m_ElementOptions.IsEmpty() ? "{}" : m_ElementOptions);
            for (uint64 i = 0; i < array.Count(); ++i)
            {
                FormatArgs(dest, i);
                elemContext.Reset();
                elemFormatter.Parse(elemContext);
                elemFormatter.FormatTo(dest, array[i]);
            }
            if (!m_RemoveBrackets)
            {
                if (m_NewLinePerElement)
                    dest.Append('\n');
                dest.Append('}');
            }
        }

    private:
        void FormatArgs(String& dest, uint64 index)
        {
            if (index > 0)
            {
                if (!m_RemoveSeparator)
                    dest.Append(m_Separator);
                if (m_NewLinePerElement)
                    dest.Append('\n');
                else if (!m_Compact)
                    dest.Append(' ');
            }
            else
            {
                if (!m_RemoveBrackets && m_NewLinePerElement)
                    dest.Append('\n');
            }
            if (m_Numbered)
            {
                Formatter<uint64> numFormatter;
                numFormatter.FormatTo(dest, index);
                if (m_Compact)
                    dest.Append(':');
                else
                    dest.Append(": ");
            }
        }

    private:
        bool m_NewLinePerElement;
        bool m_Compact;
        bool m_RemoveSeparator;
        bool m_RemoveBrackets;
        bool m_Numbered;
        char m_Separator;
        String m_ElementOptions;
    };
}
