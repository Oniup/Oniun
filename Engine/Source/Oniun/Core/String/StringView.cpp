#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/String/String.h"

namespace Oniun
{
    StringView::StringView(const String& str)
        : m_Length(str.m_Length), m_Data(str.m_Data.Ptr())
    {
    }

    // String StringView::operator+(char ch) const
    // {
    //     String result(m_Length + 1);
    //     result.Set(*this);
    //     result.Append(ch);
    //     return result;
    // }
    //
    // String StringView::operator+(const StringView& str) const
    // {
    //     String result(m_Length + str.m_Length);
    //     result.Set(*this);
    //     result.Append(str);
    //     return result;
    // }
    //
    // String StringView::operator/(char ch) const
    // {
    //     String result(m_Length + 1);
    //     result.Set(*this);
    //     result /= ch;
    //     return result;
    // }
    //
    // String StringView::operator/(const String& str) const
    // {
    //     String result(m_Length + str.m_Length);
    //     result.Set(*this);
    //     result /= str;
    //     return result;
    // }
}
