#pragma once

namespace Oniun
{
    struct IPlatform
    {
        static File GetStdOutStream();
        static File GetStdErrorStream();
    };
}
