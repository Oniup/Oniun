#pragma once

#include "Oniun/Core/String/StringView.h"
#include "Oniun/Core/String/String.h"

namespace Onu
{
    StringView::StringView(const String& str)
        : IStringView(str.m_Data.Ptr(), str.m_Length)
    {
    }

    CharStringView::CharStringView(const CharString& str)
        : IStringView(str.m_Data.Ptr(), str.m_Length)
    {
    }
}
