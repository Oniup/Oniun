#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"

#include <glm/glm.hpp>

#include "Oniun/Serialization/TypeInfo.h"

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

// TODO: HashMap, Function, LinkList

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
        /// @brief Constructs a FormatArgsContext with the given options.
        ///
        /// @param options A string view containing the format options.
        FormatArgsContext(const StringView& options);

    public:
        /// @brief Checks if the format insert arguments string is not empty and if it follows the correct insert
        ///        syntax
        ///
        /// @return True if the arguments are valid, otherwise false.
        FORCE_INLINE bool IsValid()
        {
            return !m_Args.IsEmpty() && m_Args.First() == FORMAT_OPENING_SYNTAX && m_Args.Last() == FORMAT_CLOSING_SYNTAX;
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

        /// @brief Retrieves the raw format insert arguments.
        ///
        /// @return A StringView containing the format arguments.
        FORCE_INLINE StringView Get() const
        {
            return m_Args;
        }

    private:
        StringView m_Args;
    };

    class Fmt
    {
    public:
        /// @brief Formats the given arguments into a string based on the specified format.
        ///
        /// @tparam TArgs Variadic template parameter pack for the arguments to format.
        /// @param format A string view containing the format string.
        /// @param args   The arguments to format.
        /// @return A formatted string.
        template <typename... TArgs>
        static String Format(const StringView& format, const TArgs&... args)
        {
            String result;
            FormatTo(result, format, args...);
            return result;
        }

        /// @brief Formats the given arguments and appends the result to the destination string.
        ///
        /// @tparam TArgs Variadic template parameter pack for the arguments to format.
        /// @param dest   The destination string to append the formatted result to.
        /// @param format A string view containing the format string.
        /// @param args The arguments to format.
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
            if (formatter.Parse(insert))
                formatter.FormatTo(*m_Destination, current);
            else
            {
                m_Destination->Append("{Failed Parse ");
                m_Destination->Append(TypeInfo::GetName<TCurrent>());
                m_Destination->Append('}');
            }
        }

    private:
        String* m_Destination;
        StringView m_Format;
        uint64 m_Offset;
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

#define PRIMITIVE_FORMATTER_CLASS_FIXED_SIZE_STRING(_Type) \
    template <uint64 TLength> \
    struct Formatter<_Type[TLength]> \
    { \
        FORMATTER_DEFAULT_PARSE_FUNC() \
        void FormatTo(String& dest, const char* str) \
        { \
            dest.Append(str); \
        } \
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
    PRIMITIVE_FORMATTER_CLASS_FIXED_SIZE_STRING(char)
    PRIMITIVE_FORMATTER_CLASS_FIXED_SIZE_STRING(const char)

    FORMATTER_CLASS_STRING(String)
    FORMATTER_CLASS_STRING(StringView)

#undef PRIMITIVE_FORMATTER_CLASS_SIGNED_INT
#undef PRIMITIVE_FORMATTER_CLASS_UNSIGNED_INT
#undef PRIMITIVE_FORMATTER_CLASS_FLOAT
#undef PRIMITIVE_FORMATTER_CLASS_STRING
#undef FORMATTER_CLASS_STRING

    template <typename TKey, typename TValue>
    struct Formatter<Pair<TKey, TValue>>
    {
        StringView Format;

        bool Parse(const FormatArgsContext& context)
        {
            Format = context.Get();
            return true;
        }

        void FormatTo(String& dest, const Pair<TKey, TValue>& pair)
        {
            Formatter<TKey> keyFormatter;
            Formatter<TValue> valueFormatter;
            if (!keyFormatter.Parse(Format) || !valueFormatter.Parse(Format))
            {
                dest.Append("{Failed Parse ");
                dest.Append(TypeInfo::GetName<Pair<TKey, TValue>>());
                dest.Append('}');
                return;
            }
            dest.Append('[');
            keyFormatter.FormatTo(dest, pair.Key);
            dest.Append(", ");
            valueFormatter.FormatTo(dest, pair.Value);
            dest.Append(']');
        }
    };

    template <typename T, typename TAllocationType>
    struct Formatter<Array<T, TAllocationType>>
    {
        static constexpr const char* DefaultSeparator = ",";

        /// ln
        bool NewLinePerElement = false;
        /// cp
        bool Compact = false;
        /// rs
        bool RemoveSeparator = false;
        /// rb
        bool RemoveBrackets = false;
        /// num
        bool Numbered = false;
        /// s: -> Then the single character to replace with
        char Separator = ',';
        /// Used to pass into the formatter of each element
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

    template <typename T, uint64 TLength, glm::qualifier TQualifier>
    struct Formatter<glm::vec<TLength, T, TQualifier>>
    {
        /// rb
        bool RemoveBrackets = false;
        /// ln
        bool NewLine = false;
        /// e || E -> Floating point specific
        bool ScientificNotation = false;
        /// . -> Then a number representing how many decimal points -> Floating point specific
        uint64 Precision = NO_POS;

        bool Parse(const FormatArgsContext& context)
        {
            for (StringView arg : context)
            {
                if (arg.First() == '.')
                    Precision = arg.Last() - '0';
                else if (arg == "e" || arg == "E")
                    ScientificNotation = true;
                else if (arg == "rb")
                    RemoveBrackets = true;
                else if (arg == "ln")
                    NewLine = true;
            }
            return true;
        }

        void FormatTo(String& dest, const glm::vec<TLength, T, TQualifier>& vec)
        {
            Formatter<T> valFormatter;
            if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
            {
                valFormatter.ScientificNotation = ScientificNotation;
                valFormatter.Precision = Precision;
            }
            if (!RemoveBrackets)
                dest.Append('[');
            for (uint64 i = 0; i < TLength - 1; ++i)
            {
                if (NewLine)
                    dest.Append('\n');
                valFormatter.FormatTo(dest, vec[i]);
                dest.Append(", ");
            }
            if (NewLine)
                dest.Append('\n');
            valFormatter.FormatTo(dest, vec[TLength - 1]);
            if (!RemoveBrackets)
                dest.Append(']');
        }
    };
}
