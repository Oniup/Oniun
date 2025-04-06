#pragma once

#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    enum class ShaderStage
    {
        Invalid = -1,
        Vertex,
        Fragment,
        Geometry,
        Compute,
        MaxCount,
    };

    enum class PrimitiveTopologyType
    {
        Invalid = -1,
        Point,
        Line,
        Triangle,
    };

    enum class CullMode
    {
        Normal = 0,
        Inverted,
        DoubleSided,
    };

    enum class BlendMode
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DestColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DestAlpha,
        OneMinusDestAlpha,
        ConstColor,
        OneMinusConstColor,
        ConstAlpha,
        OneMinusConstAlpha,
    };

    enum class ColorFormat
    {
        None  = 0,
        Red   = BIT(0),
        Green = BIT(1),
        Blue  = BIT(2),
        Alpha = BIT(3),
        RG = Red | Green,
        RGB = Red | Green | Blue,
        RGBA = Red | Green | Blue | Alpha,
    };

    enum class BufferDrawType
    {
        Invalid = -1,
        StaticDraw,
        DynamicDraw,
    };

    enum GpuBufferType
    {
        GpuBuffer_Invalid = -1,
        GpuBuffer_Vertex,
        GpuBuffer_Element,
        GpuBuffer_MaxCount,
    };

    template <>
    struct Formatter<ShaderStage>
    {
        FORMATTER_DEFAULT_PARSE_FUNC()
        void FormatTo(String& dest, ShaderStage type)
        {
            switch (type)
            {
            case ShaderStage::Vertex:
                dest.Append("Vertex");
                break;
            case ShaderStage::Fragment:
                dest.Append("Fragment");
                break;
            case ShaderStage::Geometry:
                dest.Append("Geometry");
                break;
            case ShaderStage::Compute:
                dest.Append("Compute");
                break;
            default:
                dest.Append("Invalid");
                break;
            }
        }
    };

    template <>
    struct Formatter<ColorFormat>
    {
        FORMATTER_DEFAULT_PARSE_FUNC()
        void FormatTo(String& dest, ColorFormat type)
        {
            switch (type)
            {
            case ColorFormat::Red:
                dest.Append("Red");
                break;
            case ColorFormat::Green:
                dest.Append("Green");
                break;
            case ColorFormat::Blue:
                dest.Append("Blue");
                break;
            case ColorFormat::Alpha:
                dest.Append("Alpha");
                break;
            case ColorFormat::RG:
                dest.Append("RG");
                break;
            case ColorFormat::RGB:
                dest.Append("RGB");
                break;
            case ColorFormat::RGBA:
                dest.Append("RGBA");
                break;
            default:
                dest.Append("Invalid");
                break;
            }
        }
    };

    template <>
    struct Formatter<BufferDrawType>
    {
        FORMATTER_DEFAULT_PARSE_FUNC()
        void FormatTo(String& dest, BufferDrawType type)
        {
            switch (type)
            {
            case BufferDrawType::StaticDraw:
                dest.Append("Static");
                break;
            case BufferDrawType::DynamicDraw:
                dest.Append("Dynamic");
                break;
            default:
                dest.Append("Invalid");
                break;
            }
        }
    };
}
