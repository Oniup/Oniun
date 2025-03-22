#include "Oniun.pch.h"
#include "Oniun/Core/Math/Color.h"

#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    String ToString(const Color32& color)
    {
        return Format("[ {}, {}, {}, {} ]", color.R, color.G, color.B, color.A);
    }

    String ToString(const Color& color)
    {
        return Format("[ {}, {}, {}, {} ]", color.R, color.G, color.B, color.A);
    }
}
