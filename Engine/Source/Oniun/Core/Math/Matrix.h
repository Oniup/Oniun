#pragma once

#include "Oniun.pch.h"

#include "Oniun/Core/Math/Vector4.h"

struct Matrix
{
    static const Matrix Zero;
    static const Matrix Identity;

    Vector4 M[4];

    Matrix();
    Matrix(const Matrix& mat);
    Matrix(const Vector4& vec0, const Vector4& vec1, const Vector4& vec2, const Vector4& vec3);
    Matrix(float x0, float y0, float z0, float w0,
           float x1, float y1, float z1, float w1,
           float x2, float y2, float z2, float w2,
           float x3, float y3, float z3, float w3);

    FORCE_INLINE Vector4& operator[](uint64 index)
    {
        return M[index];
    }

    FORCE_INLINE const Vector4& operator[](uint64 index) const
    {
        return M[index];
    }

    FORCE_INLINE Matrix& operator+=(const Matrix& mat)
    {
        *this = Add(*this, mat);
        return *this;
    }

    FORCE_INLINE Matrix operator+(const Matrix& mat)
    {
        return Add(*this, mat);
    }

    FORCE_INLINE Matrix& operator+=(float scalar)
    {
        *this = Add(*this, scalar);
        return *this;
    }

    FORCE_INLINE Matrix operator+(float scalar)
    {
        return Add(*this, scalar);
    }

    FORCE_INLINE Matrix& operator-=(const Matrix& mat)
    {
        *this = Sub(*this, mat);
        return *this;
    }

    FORCE_INLINE Matrix operator-(const Matrix& mat)
    {
        return Sub(*this, mat);
    }

    FORCE_INLINE Matrix& operator-=(float scalar)
    {
        *this = Sub(*this, scalar);
        return *this;
    }

    FORCE_INLINE Matrix operator-(float scalar)
    {
        return Sub(*this, scalar);
    }

    FORCE_INLINE Matrix& operator*=(const Matrix& mat)
    {
        *this = Mul(*this, mat);
        return *this;
    }

    FORCE_INLINE Matrix operator*(const Matrix& mat)
    {
        return Mul(*this, mat);
    }

    FORCE_INLINE Matrix& operator*=(float scalar)
    {
        *this = Mul(*this, scalar);
        return *this;
    }

    FORCE_INLINE Matrix operator*(float scalar)
    {
        return Mul(*this, scalar);
    }

    FORCE_INLINE Matrix& operator/=(const Matrix& mat)
    {
        *this = Div(*this, mat);
        return *this;
    }

    FORCE_INLINE Matrix operator/(const Matrix& mat)
    {
        return Div(*this, mat);
    }

    FORCE_INLINE Matrix& operator/=(float scalar)
    {
        *this = Div(*this, scalar);
        return *this;
    }

    FORCE_INLINE Matrix operator/(float scalar)
    {
        return Div(*this, scalar);
    }


    static Matrix Add(const Matrix& mat0, const Matrix& mat1);
    static Matrix Add(const Matrix& mat, float scalar);

    static Matrix Sub(const Matrix& mat0, const Matrix& mat1);
    static Matrix Sub(const Matrix& mat, float scalar);

    static Matrix Mul(const Matrix& mat0, const Matrix& mat1);
    static Matrix Mul(const Matrix& mat, float scalar);

    static Matrix Div(const Matrix& mat0, const Matrix& mat1);
    static Matrix Div(const Matrix& mat, float scalar);

    static Matrix Inverse(const Matrix& mat);
    static Matrix Transpose(const Matrix& mat);

    static Matrix LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
    static Matrix Perspective(float fovy, float aspect, float zNear, float zFar);
    static Matrix Orthographic(float left, float right, float bottom, float top);

    static Matrix Translate(const Matrix& mat, const Vector3& translation);
    static Matrix Scale(const Matrix& mat, const Vector3& scale);
    static Matrix Rotate(const Matrix& mat, float angle, const Vector3& rotate);
};

String ToString(const Matrix& mat);
