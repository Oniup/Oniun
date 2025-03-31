#include "Oniun/Core/Math/Vector4.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Vector3.h"

namespace Oniun
{
    const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::Up = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
    const Vector4 Vector4::Right = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    const Vector4 Vector4::Forward = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

    Vector4::Vector4()
        : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
    {
    }

    Vector4::Vector4(float val)
        : X(val), Y(val), Z(val), W(val)
    {
    }

    Vector4::Vector4(float x, float y, float z, float w)
        : X(x), Y(y), Z(z), W(w)
    {
    }

    Vector4::Vector4(const Vector4& vec)
        : X(vec.X), Y(vec.Y), Z(vec.Z), W(vec.W)
    {
    }

    Vector4::Vector4(const Vector3& vec)
        : X(vec.X), Y(vec.Y), Z(vec.Z), W(1.0f)
    {
    }

    Vector4& Vector4::operator+=(const Vector4& vec)
    {
        X += vec.X;
        Y += vec.Y;
        Z += vec.Z;
        W += vec.W;
        return *this;
    }

    Vector4 Vector4::operator+(const Vector4& vec) const
    {
        Vector4 res(*this);
        res.X += vec.X;
        res.Y += vec.Y;
        res.Z += vec.Z;
        res.W += vec.W;
        return res;
    }

    Vector4& Vector4::operator-=(const Vector4& vec)
    {
        X -= vec.X;
        Y -= vec.Y;
        Z -= vec.Z;
        W -= vec.W;
        return *this;
    }

    Vector4 Vector4::operator-(const Vector4& vec) const
    {
        Vector4 res(*this);
        res.X -= vec.X;
        res.Y -= vec.Y;
        res.Z -= vec.Z;
        res.W -= vec.W;
        return res;
    }

    Vector4& Vector4::operator*=(const Vector4& vec)
    {
        X *= vec.X;
        Y *= vec.Y;
        Z *= vec.Z;
        W *= vec.W;
        return *this;
    }

    Vector4 Vector4::operator*(const Vector4& vec) const
    {
        Vector4 res(*this);
        res.X *= vec.X;
        res.Y *= vec.Y;
        res.Z *= vec.Z;
        res.W *= vec.W;
        return res;
    }

    Vector4& Vector4::operator/=(const Vector4& vec)
    {
        for (uint64 i = 0; i < 4; ++i)
        {
            if (vec.V[i] == 0.0f)
            {
                LOG(Warning, "Cannot divide a 0 value");
                return *this;
            }
        }

        X /= vec.X;
        Y /= vec.Y;
        Z /= vec.Z;
        W /= vec.W;
        return *this;
    }

    Vector4 Vector4::operator/(const Vector4& vec) const
    {
        for (uint64 i = 0; i < 4; ++i)
        {
            if (vec.V[i] == 0.0f)
            {
                LOG(Warning, "Cannot divide a 0 value");
                return *this;
            }
        }

        Vector4 res(*this);
        res.X /= vec.X;
        res.Y /= vec.Y;
        res.Z /= vec.Z;
        res.W /= vec.W;
        return res;
    }

    Vector4& Vector4::operator*=(float scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        W *= scalar;
        return *this;
    }

    Vector4 Vector4::operator*(float scalar) const
    {
        Vector4 res(*this);
        res.X *= scalar;
        res.Y *= scalar;
        res.Z *= scalar;
        res.W *= scalar;
        return res;
    }

    Vector4& Vector4::operator/=(float scalar)
    {
        if (scalar == 0)
        {
            LOG(Warning, "Cannot divide a 0 value");
            return *this;
        }
        X /= scalar;
        Y /= scalar;
        Z /= scalar;
        W /= scalar;
        return *this;
    }

    Vector4 Vector4::operator/(float scalar) const
    {
        if (scalar == 0)
        {
            LOG(Warning, "Cannot divide a 0 value");
            return *this;
        }
        Vector4 res(*this);
        res.X /= scalar;
        res.Y /= scalar;
        res.Z /= scalar;
        res.W /= scalar;
        return res;
    }

    Vector4 operator*(float scalar, const Vector4& vec)
    {
        return vec * scalar;
    }

    Vector4 operator/(float scalar, const Vector4& vec)
    {
        return vec / scalar;
    }

    Vector4 Vector4::operator-() const
    {
        return Vector4(-X, -Y, -Z, -W);
    }

    bool Vector4::operator==(const Vector4& vec) const
    {
        return X == vec.X && Y == vec.Y && Z == vec.Z && W == vec.W;
    }

    bool Vector4::operator!=(const Vector4& vec) const
    {
        return X != vec.X && Y != vec.Y && Z != vec.Z && W != vec.W;
    }

    bool Vector4::operator<(const Vector4& vec) const
    {
        if (X == vec.X)
        {
            if (Y == vec.Y)
            {
                if (Z == vec.Z)
                    return W<vec.W;
                return Z<vec.Z;
            }
            return Y<vec.Y;
        }
        return X<vec.X;
    }

    bool Vector4::operator<=(const Vector4& vec) const
    {
        if (X == vec.X)
        {
            if (Y == vec.Y)
            {
                if (Z == vec.Z)
                    return W <= vec.W;
                return Z<vec.Z;
            }
            return Y<vec.Y;
        }
        return X<vec.X;
    }

    bool Vector4::operator>(const Vector4& vec) const
    {
        if (X == vec.X)
        {
            if (Y == vec.Y)
            {
                if (Z == vec.Z)
                    return W > vec.W;
                return Z > vec.Z;
            }
            return Y > vec.Y;
        }
        return X > vec.X;
    }

    bool Vector4::operator>=(const Vector4& vec) const
    {
        if (X == vec.X)
        {
            if (Y == vec.Y)
            {
                if (Z == vec.Z)
                    return W >= vec.W;
                return Z > vec.Z;
            }
            return Y > vec.Y;
        }
        return X > vec.X;
    }

    float Vector4::Length()
    {
        float x2 = X * X;
        float y2 = Y * Y;
        float z2 = Z * Z;
        float w2 = W * W;
        return Math::Sqrt(x2 + y2 + z2 + w2);
    }

    float Vector4::Length2()
    {
        float x2 = X * X;
        float y2 = Y * Y;
        float z2 = Z * Z;
        float w2 = W * W;
        return x2 + y2 + z2 + w2;
    }

    void Vector4::Normalize()
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
            W /= length;
        }
    }

    float Vector4::Dot(const Vector4& vec) const
    {
        return (X * vec.X) + (Y * vec.Y) * (Z * vec.Z);
    }

    float Vector4::Distance(const Vector4& vec) const
    {
        return (vec - *this).Length();
    }

    float Vector4::Distance2(const Vector4& vec) const
    {
        return (vec - *this).Length2();
    }

    Vector4 Vector4::Slide(const Vector4& normal) const
    {
        return *this - normal * Dot(normal);
    }

    Vector4 Vector4::Lerp(const Vector4& vec, float time) const
    {
        return Vector4(
            Math::Lerp(X, vec.X, time),
            Math::Lerp(Y, vec.Y, time),
            Math::Lerp(Z, vec.Z, time),
            Math::Lerp(W, vec.W, time)
        );
    }

    Vector4 Vector4::Abs() const
    {
        return Vector4(Math::Abs(X), Math::Abs(Y), Math::Abs(Z), Math::Abs(W));
    }

    Vector4 Vector4::Floor() const
    {
        return Vector4(Math::Floor(X), Math::Floor(Y), Math::Floor(Z), Math::Floor(W));
    }

    Vector4 Vector4::Ceil() const
    {
        return Vector4(Math::Ceil(X), Math::Ceil(Y), Math::Ceil(Z), Math::Ceil(W));
    }

    Vector4 Vector4::Round() const
    {
        return Vector4(Math::Round(X), Math::Round(Y), Math::Round(Z), Math::Round(W));
    }

    Vector4 Vector4::Clamp(const Vector4& min, const Vector4& max) const
    {
        return Vector4(
            Math::Clamp(X, min.X, max.X),
            Math::Clamp(Y, min.Y, max.Y),
            Math::Clamp(Z, min.Z, max.Z),
            Math::Clamp(W, min.W, max.W)
        );
    }

    Vector4 Vector4::Clamp(float min, float max) const
    {
        return Vector4(
            Math::Clamp(X, min, max),
            Math::Clamp(Y, min, max),
            Math::Clamp(Z, min, max),
            Math::Clamp(W, min, max)
        );
    }

    Vector4 Vector4::Normalized() const
    {
        Vector4 vec(*this);
        vec.Normalize();
        return vec;
    }

    String ToString(const Vector4& vec)
    {
        constexpr uint64 maxSize = 100;
        char buffer[maxSize];
        Crt::Format(buffer, maxSize, "[ %f, %f, %f, %f ]", vec.X, vec.Y, vec.Z, vec.W);
        return String(buffer);
    }

    bool Formatter<Vector4>::Parse(const FormatArgsContext& context)
    {
        for (StringView arg : context)
        {
            if (arg == "x")
                AxisPrefix = true;
            else if (arg == "rb")
                Brackets = false;
        }
        return true;
    }

    void Formatter<Vector4>::FormatTo(String& dest, const Vector4& vec)
    {
        Formatter<float> fmt;
        if (Brackets)
            dest.Append("[");

        if (AxisPrefix)
            dest.Append("X: ");
        fmt.FormatTo(dest, vec.X);
        dest.Append(", ");

        if (AxisPrefix)
            dest.Append("Y: ");
        fmt.FormatTo(dest, vec.Y);
        dest.Append(", ");

        if (AxisPrefix)
            dest.Append("Z: ");
        fmt.FormatTo(dest, vec.Z);
        dest.Append(", ");

        if (AxisPrefix)
            dest.Append("W: ");
        fmt.FormatTo(dest, vec.W);

        if (Brackets)
            dest.Append("]");
    }
}
