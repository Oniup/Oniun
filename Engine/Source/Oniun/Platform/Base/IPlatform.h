#pragma once

namespace Onu
{
    struct IPlatform
    {
        static File GetStdOutStream();
        static File GetStdErrorStream();
    };
}
