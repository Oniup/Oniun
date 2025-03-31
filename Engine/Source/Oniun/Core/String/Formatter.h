#pragma once

#include "Oniun/Core/String/String.h"

// TODO: Replace Next with an iterator
// TODO: HashMap, Pair, DateTime, LinkList, Slice, Function

namespace Oniun
{
#define FORMAT_ARG_SEPARATOR_SYNTAX '|'
#define FORMAT_ARG_CONTAINER_OPT_SYNTAX ':'

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
        bool Parse(const FormatArgsContext& context) = delete;

        /// @brief Format and append the resulting string to the destination string.
        ///
        /// @param dest  Destinationn string to push string formatted verison of type to.
        /// @param value Data of target type to stringify
        void FormatTo(String& dest, const T& value) = delete;
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
        char Brackets[2] = {'{', '}'};
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
                    else if (arg.Find("b:") != NO_POS)
                    {
                        // Forsetting bracket types in array format, the arguments must provide 2 front and back bracket
                        ASSERT(arg.Length() == 4);
                        Crt::Copy(Brackets, arg.Data() + 2, 2);
                    }
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
                dest.Append(Brackets[0]);

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
                dest.Append(Brackets[1]);
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
}
