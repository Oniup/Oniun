#pragma once

#include "Oniun/Core/String/Format.h"

namespace Oniun::RHI
{
    enum ShaderType
    {
        Invalid = -1,
        Vertex,
        Fragment,
        Geometry,
        Compute,
        MaxCount,
    };

    struct IShader
    {
        virtual ~IShader()
        {
        }

        virtual void Destroy() = 0;
        virtual bool IsValid() const = 0;
    };
}

template <>
struct Oniun::Formatter<Oniun::RHI::ShaderType>
{
    FORMATTER_DEFAULT_PARSE_FUNC()

    void FormatTo(String& dest, RHI::ShaderType type)
    {
        switch (type)
        {
        case RHI::Vertex:
            dest.Append("Vertex");
            break;
        case RHI::Fragment:
            dest.Append("Fragment");
            break;
        case RHI::Geometry:
            dest.Append("Geometry");
            break;
        case RHI::Compute:
            dest.Append("Compute");
            break;
        default:
            dest.Append("Invalid");
            break;
        }
    }
};
