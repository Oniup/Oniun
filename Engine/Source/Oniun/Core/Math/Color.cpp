#include "Oniun/Core/Math/Color.h"

#include "Oniun/Core/String/Format.h"

namespace Onu
{
    String ToString(const Color32& color)
    {
        return Format(TEXT("[ {}, {}, {}, {} ]"), color.R, color.G, color.B, color.A);
    }

    String ToString(const Color& color)
    {
        return Format(TEXT("[ {}, {}, {}, {} ]"), color.R, color.G, color.B, color.A);
    }
}
