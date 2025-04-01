#include "Oniun/Core/Math/Color.h"

#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    void Formatter<Color32>::FormatTo(String& ctx, const Color32& color)
    {
        Fmt::FormatTo(ctx, "[ {}, {}, {}, {} ]", color.R, color.G, color.B, color.A);
    }

    void Formatter<Color>::FormatTo(String& ctx, const Color& color)
    {
        Fmt::FormatTo(ctx, "[ {}, {}, {}, {} ]", color.R, color.G, color.B, color.A);
    }
}
