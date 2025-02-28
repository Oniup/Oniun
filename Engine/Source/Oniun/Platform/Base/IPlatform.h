#pragma once

struct IPlatform
{
    static File GetStdOutStream();
    static File GetStdErrorStream();
};
