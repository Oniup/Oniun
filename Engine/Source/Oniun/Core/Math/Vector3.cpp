#include "Oniun/Core/Math/Vector3.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Vector4.h"

#include <cwchar>

const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0);
const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);

Vector3::Vector3()
    : X(0.0f), Y(0.0f), Z(0.0f)
{
}

Vector3::Vector3(float val)
    : X(val), Y(val), Z(val)
{
}

Vector3::Vector3(float x, float y, float z)
    : X(x), Y(y), Z(z)
{
}

Vector3::Vector3(const Vector3& vec)
    : X(vec.X), Y(vec.Y), Z(vec.Z)
{
}

Vector3& Vector3::operator+=(const Vector3& vec)
{
    X += vec.X;
    Y += vec.Y;
    Z += vec.Z;
    return *this;
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
    Vector3 res(*this);
    res.X += vec.X;
    res.Y += vec.Y;
    res.Z += vec.Z;
    return res;
}

Vector3& Vector3::operator-=(const Vector3& vec)
{
    X -= vec.X;
    Y -= vec.Y;
    Z -= vec.Z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3& vec) const
{
    Vector3 res(*this);
    res.X -= vec.X;
    res.Y -= vec.Y;
    res.Z -= vec.Z;
    return res;
}

Vector3& Vector3::operator*=(const Vector3& vec)
{
    X *= vec.X;
    Y *= vec.Y;
    Z *= vec.Z;
    return *this;
}

Vector3 Vector3::operator*(const Vector3& vec) const
{
    Vector3 res(*this);
    res.X *= vec.X;
    res.Y *= vec.Y;
    res.Z *= vec.Z;
    return res;
}

Vector3& Vector3::operator/=(const Vector3& vec)
{
    for (uint64 i = 0; i < 4; ++i)
    {
        if (vec.Raw[i] == 0.0f)
        {
            LOG(Warning, "Cannot divide a 0 value");
            return *this;
        }
    }

    X /= vec.X;
    Y /= vec.Y;
    Z /= vec.Z;
    return *this;
}

Vector3 Vector3::operator/(const Vector3& vec) const
{
    for (uint64 i = 0; i < 4; ++i)
    {
        if (vec.Raw[i] == 0.0f)
        {
            LOG(Warning, "Cannot divide a 0 value");
            return *this;
        }
    }

    Vector3 res(*this);
    res.X /= vec.X;
    res.Y /= vec.Y;
    res.Z /= vec.Z;
    return res;
}

Vector3& Vector3::operator*=(float scalar)
{
    X *= scalar;
    Y *= scalar;
    Z *= scalar;
    return *this;
}

Vector3 Vector3::operator*(float scalar) const
{
    Vector3 res(*this);
    res.X *= scalar;
    res.Y *= scalar;
    res.Z *= scalar;
    return res;
}

Vector3& Vector3::operator/=(float scalar)
{
    if (scalar == 0)
    {
        LOG(Warning, "Cannot divide a 0 value");
        return *this;
    }
    X /= scalar;
    Y /= scalar;
    Z /= scalar;
    return *this;
}

Vector3 Vector3::operator/(float scalar) const
{
    if (scalar == 0)
    {
        LOG(Warning, "Cannot divide a 0 value");
        return *this;
    }
    Vector3 res(*this);
    res.X /= scalar;
    res.Y /= scalar;
    res.Z /= scalar;
    return res;
}

Vector3 operator*(float scalar, const Vector3& vec)
{
    return vec * scalar;
}

Vector3 operator/(float scalar, const Vector3& vec)
{
    return vec / scalar;
}

Vector3 Vector3::operator-() const
{
    return Vector3(-X, -Y, -Z);
}

bool Vector3::operator==(const Vector3& vec) const
{
    return X == vec.X && Y == vec.Y && Z == vec.Z;
}

bool Vector3::operator!=(const Vector3& vec) const
{
    return X != vec.X && Y != vec.Y && Z != vec.Z;
}

bool Vector3::operator<(const Vector3& vec) const
{
    if (X == vec.X)
    {
        if (Y == vec.Y)
            return Z<vec.Z;
        return Y<vec.Y;
    }
    return X<vec.X;
}

bool Vector3::operator<=(const Vector3& vec) const
{
    if (X == vec.X)
    {
        if (Y == vec.Y)
            return Z <= vec.Z;
        return Y<vec.Y;
    }
    return X<vec.X;
}

bool Vector3::operator>(const Vector3& vec) const
{
    if (X == vec.X)
    {
        if (Y == vec.Y)
            return Z > vec.Z;
        return Y > vec.Y;
    }
    return X > vec.X;
}

bool Vector3::operator>=(const Vector3& vec) const
{
    if (X == vec.X)
    {
        if (Y == vec.Y)
            return Z >= vec.Z;
        return Y > vec.Y;
    }
    return X > vec.X;
}

float Vector3::Length()
{
    float x2 = X * X;
    float y2 = Y * Y;
    float z2 = Z * Z;
    return Math::Sqrt(x2 + y2 + z2);
}

float Vector3::Length2()
{
    float x2 = X * X;
    float y2 = Y * Y;
    float z2 = Z * Z;
    return x2 + y2 + z2;
}

void Vector3::Normalize()
{
    float length = Length2();
    if (length == 0)
        X = Y = Z = 0;
    else
    {
        length = Length();
        X /= length;
        Y /= length;
        Z /= length;
    }
}

float Vector3::Dot(const Vector3& vec) const
{
    return (X * vec.X) + (Y * vec.Y) * (Z * vec.Z);
}

float Vector3::Distance(const Vector3& vec) const
{
    return (vec - *this).Length();
}

float Vector3::Distance2(const Vector3& vec) const
{
    return (vec - *this).Length2();
}

Vector3 Vector3::Cross(const Vector3& vec)
{
    return Vector3(
        (Y * vec.Z) - (Z * vec.Y),
        (Z * vec.X) - (X * vec.Z),
        (X * vec.Y) - (Y * vec.X)
    );
}

Vector3 Vector3::Reflect(const Vector3& normal) const
{
    return 2.0f * normal * Dot(normal) - *this;
}

Vector3 Vector3::Bounce(const Vector3& normal) const
{
    return -Reflect(normal);
}

Vector3 Vector3::Slide(const Vector3& normal) const
{
    return *this - normal * Dot(normal);
}

Vector3 Vector3::Lerp(const Vector3& vec, float time) const
{
    return Vector3(
        Math::Lerp(X, vec.X, time),
        Math::Lerp(Y, vec.Y, time),
        Math::Lerp(Z, vec.Z, time)
    );
}

Vector3 Vector3::Abs() const
{
    return Vector3(Math::Abs(X), Math::Abs(Y), Math::Abs(Z));
}

Vector3 Vector3::Floor() const
{
    return Vector3(Math::Floor(X), Math::Floor(Y), Math::Floor(Z));
}

Vector3 Vector3::Ceil() const
{
    return Vector3(Math::Ceil(X), Math::Ceil(Y), Math::Ceil(Z));
}

Vector3 Vector3::Round() const
{
    return Vector3(Math::Round(X), Math::Round(Y), Math::Round(Z));
}

Vector3 Vector3::Clamp(const Vector3& min, const Vector3& max) const
{
    return Vector3(
        Math::Clamp(X, min.X, max.X),
        Math::Clamp(Y, min.Y, max.Y),
        Math::Clamp(Z, min.Z, max.Z)
    );
}

Vector3 Vector3::Clamp(float min, float max) const
{
    return Vector3(
        Math::Clamp(X, min, max),
        Math::Clamp(Y, min, max),
        Math::Clamp(Z, min, max)
    );
}

String ToString(const Vector3& vec)
{
    char buffer[100];
    snprintf(buffer, 100, "[ %f, %f, %f ]", vec.X, vec.Y, vec.Z);
    return String(buffer);
}

Vector3 Vector3::Normalized() const
{
    Vector3 vec(*this);
    vec.Normalize();
    return vec;
}
