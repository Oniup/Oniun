#pragma once

#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/String/String.h"

namespace Onu
{
    StringView::StringView(const String& str)
        : IStringView(str.m_Data.Ptr(), str.m_Length)
    {
    }

    String StringView::operator+(Char ch) const
    {
        String result(m_Length + 1);
        result.Set(*this);
        result.Append(ch);
        return result;
    }

    String StringView::operator+(const StringView& str) const
    {
        String result(m_Length + str.m_Length);
        result.Set(*this);
        result.Append(str);
        return result;
    }

    String StringView::operator/(Char ch) const
    {
        String result(m_Length + 1);
        result.Set(*this);
        result /= ch;
        return result;
    }

    String StringView::operator/(const String& str) const
    {
        String result(m_Length + str.m_Length);
        result.Set(*this);
        result /= str;
        return result;
    }

    CharStringView::CharStringView(const CharString& str)
        : IStringView(str.m_Data.Ptr(), str.m_Length)
    {
    }

    CharString CharStringView::operator+(char ch) const
    {
        CharString result(m_Length + 1);
        result.Set(*this);
        result.Append(ch);
        return result;
    }

    CharString CharStringView::operator+(const CharStringView& str) const
    {
        CharString result(m_Length + str.m_Length);
        result.Append(*this);
        result.Append(str);
        return result;
    }

    CharString CharStringView::operator/(char ch) const
    {
        CharString result(m_Length + 1);
        result.Set(*this);
        result /= ch;
        return result;
    }

    CharString CharStringView::operator/(const CharStringView& str) const
    {
        CharString result(m_Length + str.m_Length);
        result.Set(*this);
        result /= str;
        return result;
    }
}
