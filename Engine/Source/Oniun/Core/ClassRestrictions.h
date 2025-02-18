#pragma once

namespace Onu
{
    struct INonCopyable
    {
        INonCopyable() = default;
        INonCopyable(const INonCopyable& other) = delete;
        INonCopyable& operator=(INonCopyable& other) = delete;
    };

    struct INonMoveable
    {
        INonMoveable() = default;
        INonMoveable(INonMoveable&& other) = delete;
        INonMoveable& operator=(INonMoveable&& other) = delete;
    };
}
