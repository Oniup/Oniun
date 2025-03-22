#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/String/StringUtils.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"

#define FORMAT_SYNTAX "{}"

namespace Oniun
{
    String ToString(int64 val);
    String ToString(int32 val);
    String ToString(int16 val);
    String ToString(int8 val);

    String ToString(uint64 val);
    String ToString(uint32 val);
    String ToString(uint16 val);
    String ToString(uint8 val);

    String ToString(float val);
    String ToString(double val);

    FORCE_INLINE StringView ToString(const StringView& val)
    {
        return val;
    }

    template <typename T, typename TAllocationType>
    String ToString(const Array<T, TAllocationType>& array, bool sameLine = true)
    {
        String result(array.Count() * sizeof(T));
        result.Append("[");
        bool first = true;
        for (const T& val : array)
        {
            if (first)
            {
                first = false;
                result.Append(StringView(sameLine ? " " : "\n"));
            }
            else
                result.Append(StringView(sameLine ? ", " : "\n"));
            result.Append(ToString(val));
        }
        result.Append(sameLine ? " ]" : "\n]");
        return result;
    }

    template <typename TKey, typename TValue, typename THash, typename TAllocationType>
    String ToString(const HashMap<TKey, TValue, THash, TAllocationType>& map, bool sameLine = true)
    {
        String result(map.Count() * sizeof(TKey) + sizeof(TValue));
        result.Append("[");
        bool first = true;
        for (const auto& bucket : map)
        {
            if (first)
            {
                first = false;
                result.Append((sameLine ? " { " : "\n{ "));
            }
            else
                result.Append((sameLine ? ", { " : "\n{ "));
            result.Append(ToString(bucket.Value) + StringView(", ")) + ToString(bucket.GetKey()) + " }";
        }
        result.Append(sameLine ? " ]" : "\n]");
        return result;
    }

    namespace Format_Internal
    {
        ///Appends a formatted argument to the result string based on the current format string and insert positions.
        ///
        /// @tparam T               Target element type from the template parameter pack.
        /// @param format           String defining the format position of each argument in parameter pack.
        /// @param insertPositions  Array of insert positions defined by the format string.
        /// @param result           String to add resulting formatted string.
        /// @param i                Current insert position index.
        /// @param offset           Current offset in the format string.
        /// @param arg              The current argument to format and append.
        template <typename T>
        FORCE_INLINE void Add(const StringView& format, const Array<uint64>& insertPositions, String& result, uint64& i,
                              uint64& offset, const T& arg)
        {
            Slice slice(format.begin() + offset, format.begin() + insertPositions[i]);
            result.Append(slice);
            result.Append(ToString(arg));
            offset = insertPositions[i] + StringUtils::Length(FORMAT_SYNTAX);
            i++;
        }

        /// Processes the next argument in the format string.
        ///
        /// @tparam T               Target element type from the template parameter pack.
        /// @param format           String defining the format position of each argument in parameter pack.
        /// @param insertPositions  Array of insert positions defined by the format string.
        /// @param result           String to add resulting formatted string.
        /// @param i                Current insert position index.
        /// @param offset           Current offset in the format string.
        /// @param current          The current argument to format and append.
        template <typename T>
        void Next(const StringView& format, const Array<uint64>& insertPositions, String& result, uint64& i, uint64& offset,
                  const T& current)
        {
            Add(format, insertPositions, result, i, offset, current);
        }

        /// Processes the next arguments in the format string.
        ///
        /// @tparam T               Target element type from the template parameter pack.
        /// @tparam TArgs           Variadic template for additional arguments.
        /// @param format           String defining the format position of each argument in parameter pack.
        /// @param insertPositions  Array of insert positions defined by the format string.
        /// @param result           String to add resulting formatted string.
        /// @param i                Current insert position index.
        /// @param offset           Current offset in the format string.
        /// @param current          The current argument to format and append.
        /// @param args             Additional arguments to format and append.
        template <typename T, typename... TArgs>
        void Next(const StringView& format, const Array<uint64>& insertPositions, String& result, uint64& i, uint64& offset,
                  const T& current, const TArgs&... args)
        {
            Add(format, insertPositions, result, i, offset, current);
            Next(format, insertPositions, result, i, offset, args...);
        }
    }

    /// Formats a string based on the provided format string and arguments.
    ///
    /// @tparam TArgs   Variadic template for the arguments to format.
    /// @param format   The format string containing placeholders.
    /// @param args     The arguments to insert into the format string.
    /// @return A formatted String with the arguments inserted at the specified positions.
    template <typename... TArgs>
    String Format(const StringView& format, const TArgs&... args)
    {
        Array<uint64> insertPositions;
        if (!format.FindAll(FORMAT_SYNTAX, insertPositions))
            return format;

        String result;
        uint64 i = 0;
        uint64 offset = 0;
        Format_Internal::Next(format, insertPositions, result, i, offset, args...);

        // Add the rest of the format string if there is any left over
        Slice rest(format.begin() + offset, format.end());
        result.Append(rest);
        return result;
    }
}
