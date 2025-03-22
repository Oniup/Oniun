#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/String.h"

namespace Oniun
{
    namespace Color_Internal
    {
        constexpr uint8 HexToInt(const char* hex)
        {
            constexpr auto calc = [](char ch) -> uint8
            {
                if (ch >= '0' && ch <= '9')
                    return (uint8)(ch - '0');
                if (ch >= 'a' && ch <= 'f')
                    return (uint8)(ch - 'a' + 10);
                if (ch >= 'A' && ch <= 'F')
                    return (uint8)(ch - 'A' + 10);
                return 0;
            };
            return calc(hex[0]) * 16 + calc(hex[1]);
        }
    }

    struct Color;

    struct Color32
    {
        union
        {
            struct
            {
                uint8 R;
                uint8 G;
                uint8 B;
                uint8 A;
            };

            uint32 Base;
        };

        constexpr Color32(uint32 color32)
            : Base(color32)
        {
        }

        constexpr Color32()
            : R(0), G(0), B(0), A(0)
        {
        }

        constexpr Color32(uint8 red, uint8 green, uint8 blue, uint8 alpha = 255)
            : R(red), G(green), B(blue), A(alpha)
        {
        }

        constexpr Color32(const StringView& hexadecimal, uint8 alpha = 255)
            : Color32()
        {
            if (hexadecimal.Length() >= 6)
            {
                uint64 hash = hexadecimal.First() == '#' ? 1 : 0;
                R = Color_Internal::HexToInt(hexadecimal.Data() + hash);
                G = Color_Internal::HexToInt(hexadecimal.Data() + hash + 2);
                B = Color_Internal::HexToInt(hexadecimal.Data() + hash + 4);
                A = alpha;
            }
        }

        constexpr Color32(const Color& color);
    };

    struct Color
    {
        union
        {
            struct
            {
                float R;
                float G;
                float B;
                float A;
            };

            float Base[4];
        };

        constexpr Color()
            : R(0.0f), G(0.0f), B(0.0f), A(0.0f)
        {
        }

        constexpr Color(float red, float green, float blue, float alpha = 1.0)
            : R(red), G(green), B(blue), A(alpha)
        {
        }

        constexpr Color(const Color32& color)
            : R(color.R / 255.0f), G(color.G / 255.0f), B(color.B / 255.0f), A(color.A / 255.0f)
        {
        }

        constexpr Color(const Color& color)
            : R(color.R), G(color.G), B(color.B), A(color.A)
        {
        }

        constexpr Color(const StringView& hexadecimal, float alpha = 1.0f)
        {
            if (hexadecimal.Length() >= 6)
            {
                uint64 hash = hexadecimal[0] == '#' ? 1 : 0;
                R = Color_Internal::HexToInt(hexadecimal.Data() + hash) / 255.0f;
                G = Color_Internal::HexToInt(hexadecimal.Data() + hash + 2) / 255.0f;
                B = Color_Internal::HexToInt(hexadecimal.Data() + hash + 4) / 255.0f;
                A = alpha;
            }
        }

        constexpr Color& operator=(const Color& color)
        {
            for (uint64 i = 0; i < 4; ++i)
            {
                Base[i] = color.Base[i];
            }
            return *this;
        }
    };

    constexpr Color32::Color32(const Color& color)
        : R((uint8)(color.R * 255)), G((uint8)(color.G * 255)), B((uint8)(color.B * 255)), A((uint8)(color.A * 255))
    {
    }

    static constexpr Color Black = Color("#000000");
    static constexpr Color White = Color("#FFFFFF");
    static constexpr Color Red = Color("#FF0000");
    static constexpr Color Green = Color("#00FF00");
    static constexpr Color Blue = Color("#0000FF");
    static constexpr Color Yellow = Color("#FFFF00");
    static constexpr Color Cyan = Color("#00FFFF");
    static constexpr Color Magenta = Color("#FF00FF");
    static constexpr Color Silver = Color("#C0C0C0");
    static constexpr Color Gray = Color("#808080");
    static constexpr Color Maroon = Color("#800000");
    static constexpr Color Olive = Color("#808000");
    static constexpr Color Purple = Color("#800080");
    static constexpr Color Teal = Color("#008080");
    static constexpr Color Navy = Color("#000080");
    static constexpr Color Orange = Color("#FFA500");
    static constexpr Color Pink = Color("#FFC0CB");
    static constexpr Color DarkGray = Color("#A9A9A9");
    static constexpr Color DarkRed = Color("#8B0000");
    static constexpr Color LightGray = Color("#D3D3D3");
    static constexpr Color LightBlue = Color("#ADD8E6");
    static constexpr Color LightGreen = Color("#90EE90");
    static constexpr Color LightCoral = Color("#F08080");
    static constexpr Color LightSalmon = Color("#FFA07A");
    static constexpr Color LightGoldenrodYellow = Color("#FAFAD2");
    static constexpr Color LightSkyBlue = Color("#87CEFA");
    static constexpr Color LightSeaGreen = Color("#20B2AA");
    static constexpr Color LightSlateGray = Color("#778899");
    static constexpr Color LightSteelBlue = Color("#B0C4DE");
    static constexpr Color PaleGoldenrod = Color("#EEE8AA");
    static constexpr Color PaleGreen = Color("#98FB98");
    static constexpr Color PaleTurquoise = Color("#AFEEEE");
    static constexpr Color PaleVioletRed = Color("#DB7093");
    static constexpr Color Salmon = Color("#FA8072");
    static constexpr Color SandyBrown = Color("#F4A460");
    static constexpr Color SeaGreen = Color("#2E8B57");
    static constexpr Color SlateBlue = Color("#6A5ACD");
    static constexpr Color SlateGray = Color("#708090");
    static constexpr Color SpringGreen = Color("#00FF7F");
    static constexpr Color Tomato = Color("#FF6347");
    static constexpr Color Turquoise = Color("#40E0D0");
    static constexpr Color Violet = Color("#EE82EE");
    static constexpr Color Wheat = Color("#F5DEB3");
    static constexpr Color YellowGreen = Color("#9ACD32");

    String ToString(const Color32& color);
    String ToString(const Color& color);
}
