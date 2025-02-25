#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Math/Math.h"

namespace Onu
{
    struct Vector3
    {
        enum
        {
            Size = 3
        };

        union
        {
            struct
            {
                float X;
                float Y;
                float Z;
            };
            float Raw[3];
        };

        Vector3();
        Vector3(float val);
        Vector3(float x, float y, float z);
        Vector3(const Vector3& vec);

        Vector3& operator+=(const Vector3& vec);
        Vector3 operator+(const Vector3& vec) const;
        Vector3& operator-=(const Vector3& vec);
        Vector3 operator-(const Vector3& vec) const;
        Vector3& operator*=(const Vector3& vec);
        Vector3 operator*(const Vector3& vec) const;
        Vector3& operator/=(const Vector3& vec);
        Vector3 operator/(const Vector3& vec) const;

        Vector3& operator*=(float scalar);
        Vector3 operator*(float scalar) const;
        Vector3& operator/=(float scalar);
        Vector3 operator/(float scalar) const;

        friend Vector3 operator*(float scalar, const Vector3& vec);
        friend Vector3 operator/(float scalar, const Vector3& vec);

        Vector3 operator-() const;

        bool operator==(const Vector3& vec) const;
        bool operator!=(const Vector3& vec) const;
        bool operator<(const Vector3& vec) const;
        bool operator<=(const Vector3& vec) const;
        bool operator>(const Vector3& vec) const;
        bool operator>=(const Vector3& vec) const;

        float Length();
        float Length2();
        void Normalize();

        float Dot(const Vector3& vec) const;
        float Distance(const Vector3& vec) const;
        float Distance2(const Vector3& vec) const;

        Vector3 Cross(const Vector3& vec);
        Vector3 Normalized() const;
        Vector3 Reflect(const Vector3& normal) const;
        Vector3 Bounce(const Vector3& normal) const;
        Vector3 Slide(const Vector3& normal) const;
        Vector3 Lerp(const Vector3& vec, float time) const;

        Vector3 Abs() const;
        Vector3 Floor() const;
        Vector3 Ceil() const;
        Vector3 Round() const;
        Vector3 Clamp(const Vector3& min, const Vector3& max) const;
        Vector3 Clamp(float min, float max) const;
    };
}
