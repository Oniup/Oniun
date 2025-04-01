#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"

#define FORMAT_SYNTAX "{}"
#define FORMAT_OPENING_SYNTAX '{'
#define FORMAT_CLOSING_SYNTAX '}'
#define FORMAT_ARG_SEPARATOR_SYNTAX '|'
#define FORMAT_ARG_CONTAINER_OPT_SYNTAX ':'

#define FORMATTER_DEFAULT_PARSE_FUNC() \
    FORCE_INLINE bool Parse(const Oniun::FormatArgsContext& context) \
    { \
        return true; \
    }

namespace Oniun
{
    template <typename T>
    struct Formatter
    {
        /// @brief Here you parse the arguments passed within the {} and store the options within the formatter to be
        ///        used when FormatTo(...) is called.
        /// @note  If you don't need to parse any options just return true.
        ///
        /// @param context Format option arguments for formatting the type.
        /// @return True if should continue to format into a string, otherwise parse failed and should append into
        ///         destination string.
        bool Parse(const class FormatArgsContext& context) = delete;

        /// @brief Format and append the resulting string to the destination string.
        ///
        /// @param dest  Destination string to push string formatted version of type to.
        /// @param value Data of target type to stringify
        void FormatTo(String& dest, const T& value) = delete;
    };

    class FormatArgsContext
    {
    public:
        class ArgIterator
        {
        public:
            ArgIterator();
            ArgIterator(FormatArgsContext* context, uint64 begin, uint64 end);
            ArgIterator(const ArgIterator& iter);
            ArgIterator(ArgIterator&& iter);

        public:
            ArgIterator& operator=(const ArgIterator& iter);
            ArgIterator& operator=(ArgIterator&& iter);

            StringView operator*() const;

            bool operator==(const ArgIterator& iter) const;
            bool operator!=(const ArgIterator& iter) const;

            ArgIterator& operator++();

        public:
            FORCE_INLINE StringView Get() const
            {
                return StringView(m_Context->m_Args.Data() + m_CurrentBegin, m_CurrentEnd - m_CurrentBegin);
            }

        private:
            FormatArgsContext* m_Context;
            uint64 m_CurrentBegin;
            uint64 m_CurrentEnd;
        };

    public:
        FormatArgsContext(const StringView& options);

    public:
        FORCE_INLINE bool IsValid()
        {
            return !m_Args.IsEmpty();
        }

        FORCE_INLINE ArgIterator begin() const
        {
            ArgIterator iter(const_cast<FormatArgsContext*>(this), 0, 0);
            ++iter;
            return iter;
        }

        FORCE_INLINE ArgIterator end() const
        {
            ArgIterator iter(const_cast<FormatArgsContext*>(this), NO_POS, NO_POS);
            ++iter;
            return iter;
        }

        FORCE_INLINE ArgIterator Begin() const
        {
            return begin();
        }

        FORCE_INLINE ArgIterator End() const
        {
            return end();
        }

    private:
        StringView m_Args;
    };

#define PRIMITIVE_FORMATTER_CLASS_FLOAT(_Type) \
    template <> \
    struct Formatter<_Type> \
    { \
        bool ScientificNotation = false; \
        uint64 Precision = NO_POS; \
        bool Parse(const FormatArgsContext& context); \
        void FormatTo(String& dest, _Type value); \
    };

#define PRIMITIVE_FORMATTER_CLASS_SIGNED_INT(_Bits) \
    template <> \
    struct Formatter<int##_Bits> \
    { \
        bool Pointer = false; \
        bool Parse(const FormatArgsContext& context); \
        void FormatTo(String& dest, int##_Bits value); \
    };

#define PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT(_Bits) \
    template <> \
    struct Formatter<uint##_Bits> \
    { \
        bool Hexadecimal = false; \
        bool Pointer = false; \
        bool Parse(const FormatArgsContext& context); \
        void FormatTo(String& dest, uint##_Bits value); \
    };

#define PRIMITIVE_FORMATTER_CLASS_STRING(_Type) \
    template <> \
    struct Formatter<_Type> \
    { \
        bool Parse(const FormatArgsContext& context);  \
        void FormatTo(String& dest, const char* str);  \
    };

#define FORMATTER_CLASS_STRING(_Type) \
    template <> \
    struct Formatter<_Type> \
    { \
        bool Parse(const FormatArgsContext& context); \
        void FormatTo(String& dest, const _Type& str); \
    };

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

    PRIMITIVE_FORMATTER_CLASS_STRING(char*)
    PRIMITIVE_FORMATTER_CLASS_STRING(const char*)

    FORMATTER_CLASS_STRING(String)
    FORMATTER_CLASS_STRING(StringView)

#undef PRIMITIVE_FORMATTER_CLASS_SIGNED_INT
#undef PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT
#undef PRIMITIVE_FORMATTER_CLASS_FLOAT
#undef PRIMITIVE_FORMATTER_CLASS_STRING
#undef FORMATTER_CLASS_STRING

    template <uint64 TLength>
    struct Formatter<char[TLength]>
    {
        FORMATTER_DEFAULT_PARSE_FUNC()

        void FormatTo(String& dest, const char* str)
        {
            dest.Append(str);
        }
    };

    template <typename T, typename TAllocationType>
    struct Formatter<Array<T, TAllocationType>>
    {
        static constexpr const char* DefaultSeparator = ",";

        bool NewLinePerElement = false;
        bool Compact = false;
        bool RemoveSeparator = false;
        bool RemoveBrackets = false;
        bool Numbered = false;
        char Separator = ',';
        String ElementOptions;

        bool Parse(const FormatArgsContext& context)
        {
            bool isArrOptions = true;
            for (StringView arg : context)
            {
                if (isArrOptions)
                {
                    if (arg == "ln")
                        NewLinePerElement = true;
                    else if (arg == "cp")
                        Compact = true;
                    else if (arg == "rs")
                        RemoveSeparator = true;
                    else if (arg == "rb")
                        RemoveBrackets = true;
                    else if (arg == "num")
                        Numbered = true;
                    else if (arg.Find("s:") != NO_POS)
                        Separator = arg.Last();
                    else if (arg == "elem")
                        isArrOptions = false;
                }
                else
                {
                    bool first = ElementOptions.IsEmpty();
                    if (first)
                        ElementOptions.Append('{');
                    else
                        ElementOptions.Append(FORMAT_ARG_SEPARATOR_SYNTAX);
                    ElementOptions.Append(arg);
                }
            }
            if (!ElementOptions.IsEmpty())
                ElementOptions.Append('}');
            return true;
        }

        void FormatTo(String& dest, const Array<T, TAllocationType>& array)
        {
            if (!RemoveBrackets)
                dest.Append('{');

            Formatter<T> elemFormatter;
            FormatArgsContext elemContext(ElementOptions.IsEmpty() ? "{}" : ElementOptions);
            for (uint64 i = 0; i < array.Count(); ++i)
            {
                FormatArgs(dest, i);
                elemFormatter.Parse(elemContext);
                elemFormatter.FormatTo(dest, array[i]);
            }
            if (!RemoveBrackets)
            {
                if (NewLinePerElement)
                    dest.Append('\n');
                dest.Append('}');
            }
        }

    private:
        void FormatArgs(String& dest, uint64 index)
        {
            if (index > 0)
            {
                if (!RemoveSeparator)
                    dest.Append(Separator);
                if (NewLinePerElement)
                    dest.Append('\n');
                else if (!Compact)
                    dest.Append(' ');
            }
            else
            {
                if (!RemoveBrackets && NewLinePerElement)
                    dest.Append('\n');
            }
            if (Numbered)
            {
                Formatter<uint64> numFormatter;
                numFormatter.FormatTo(dest, index);
                if (Compact)
                    dest.Append(':');
                else
                    dest.Append(": ");
            }
        }
    };

    class Fmt
    {
    public:
        template <typename... TArgs>
        static String Format(const StringView& format, const TArgs&... args)
        {
            String result;
            FormatTo(result, format, args...);
            return result;
        }

        template <typename... TArgs>
        static void FormatTo(String& dest, const StringView& format, const TArgs&... args)
        {
            Fmt fmt(&dest, format);
            fmt.Next(args...);
        }

    private:
        Fmt(String* dest, const StringView& format)
            : m_Destination(dest), m_Format(format), m_Offset(0)
        {
        }

    private:
        void FindNextInsert(StringView& prefix, StringView& insert)
        {
            // Iterate through until you find a opening and closing bracket
            uint64 begin = NO_POS;
            uint64 i = m_Offset;
            for (; i < m_Format.Length(); ++i)
            {
                if (m_Format[i] == FORMAT_OPENING_SYNTAX)
                {
                    // Got two opening brackets in a row thus is not valid
                    if (i != 0 && begin == i - 1)
                    {
                        prefix = StringView(m_Format.Data() + m_Offset, i);
                        m_Offset = i + 1;
                        return;
                    }
                    begin = i;
                }
                if (begin != NO_POS && m_Format[i] == FORMAT_CLOSING_SYNTAX)
                    break;
            }
            // If insert has been found
            if (begin != NO_POS)
            {
                insert = StringView(m_Format.Data() + begin, i - begin + 1);
                prefix = StringView(m_Format.Begin() + m_Offset, m_Format.Begin() + begin);
                m_Offset = i + 1;
            }
        }

        template <typename TCurrent, typename... TRest>
        void Next(const TCurrent& current, const TRest&... rest)
        {
            Append(current);
            Next(rest...);
        }

        template <typename TCurrent>
        void Next(const TCurrent& current)
        {
            Append(current);
            if (m_Offset < m_Format.Length())
                m_Destination->Append(StringView(m_Format.Data() + m_Offset));
        }

        template <typename TCurrent>
        void Append(const TCurrent& current)
        {
            StringView prefix, insert;
            FindNextInsert(prefix, insert);
            m_Destination->Append(prefix);
            if (insert.IsEmpty())
            {
                Append(current);
                return;
            }
            // TODO: Run formatter of TCurrent
            Formatter<TCurrent> formatter;
            formatter.Parse(insert);
            formatter.FormatTo(*m_Destination, current);
        }

    private:
        String* m_Destination;
        StringView m_Format;
        uint64 m_Offset;
    };
}
