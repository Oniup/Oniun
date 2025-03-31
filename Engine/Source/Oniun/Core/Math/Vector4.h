#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Math/Math.h"
#include "Oniun/Core/String/Formatter.h"

namespace Oniun
{
    struct Vector4
    {
        static const Vector4 Zero;
        static const Vector4 Up;
        static const Vector4 Right;
        static const Vector4 Forward;

        union
        {
            struct
            {
                float X;
                float Y;
                float Z;
                float W;
            };

            float V[4];
        };

        Vector4();
        Vector4(float val);
        Vector4(float x, float y, float z, float w);
        Vector4(const Vector4& vec);
        Vector4(const Vector3& vec);

        FORCE_INLINE float& operator[](uint64 index)
        {
            return V[index];
        }

        FORCE_INLINE float operator[](uint64 index) const
        {
            return V[index];
        }

        Vector4& operator+=(const Vector4& vec);
        Vector4 operator+(const Vector4& vec) const;
        Vector4& operator-=(const Vector4& vec);
        Vector4 operator-(const Vector4& vec) const;
        Vector4& operator*=(const Vector4& vec);
        Vector4 operator*(const Vector4& vec) const;
        Vector4& operator/=(const Vector4& vec);
        Vector4 operator/(const Vector4& vec) const;

        Vector4& operator*=(float scalar);
        Vector4 operator*(float scalar) const;
        Vector4& operator/=(float scalar);
        Vector4 operator/(float scalar) const;

        friend Vector4 operator*(float scalar, const Vector4& vec);
        friend Vector4 operator/(float scalar, const Vector4& vec);

        Vector4 operator-() const;

        bool operator==(const Vector4& vec) const;
        bool operator!=(const Vector4& vec) const;
        bool operator<(const Vector4& vec) const;
        bool operator<=(const Vector4& vec) const;
        bool operator>(const Vector4& vec) const;
        bool operator>=(const Vector4& vec) const;

        float Length();
        float Length2();
        void Normalize();

        float Dot(const Vector4& vec) const;
        float Distance(const Vector4& vec) const;
        float Distance2(const Vector4& vec) const;

        Vector4 Normalized() const;
        Vector4 Slide(const Vector4& normal) const;
        Vector4 Lerp(const Vector4& vec, float time) const;

        Vector4 Abs() const;
        Vector4 Floor() const;
        Vector4 Ceil() const;
        Vector4 Round() const;
        Vector4 Clamp(const Vector4& min, const Vector4& max) const;
        Vector4 Clamp(float min, float max) const;
    };

    template <>
    struct Formatter<Vector4>
    {
        bool Brackets = true;
        bool AxisPrefix = false;

        bool Parse(const FormatArgsContext& context);
        void FormatTo(String& dest, const Vector4& vec);
    };

    String ToString(const Vector4& vec);
}
