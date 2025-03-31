#include "Oniun/Core/Math/Matrix.h"

#include "Oniun/Core/Math/Vector3.h"
#include "Oniun/Core/String/Format.h"

namespace Oniun
{
    const Matrix Matrix::Zero = Matrix(
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    );

    const Matrix Matrix::Identity = Matrix(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    Matrix::Matrix()
        : Matrix(0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    Matrix::Matrix(const Matrix& mat)
        : M{
            mat.M[0],
            mat.M[1],
            mat.M[2],
            mat.M[3]
        }
    {
    }

    Matrix::Matrix(const Vector4& vec0, const Vector4& vec1, const Vector4& vec2, const Vector4& vec3)
        : M{
            vec0,
            vec1,
            vec2,
            vec3
        }
    {
    }

    Matrix::Matrix(float x0, float y0, float z0, float w0,
                   float x1, float y1, float z1, float w1,
                   float x2, float y2, float z2, float w2,
                   float x3, float y3, float z3, float w3)
        : M{
            Vector4(x0, y0, z0, w0),
            Vector4(x1, y1, z1, w1),
            Vector4(x2, y2, z2, w2),
            Vector4(x3, y3, z3, w3)
        }
    {
    }

    Matrix Matrix::Add(const Matrix& mat0, const Matrix& mat1)
    {
        Matrix result(mat0);
        result.M[0] += mat1.M[0];
        result.M[1] += mat1.M[1];
        result.M[2] += mat1.M[2];
        result.M[3] += mat1.M[3];
        return result;
    }

    Matrix Matrix::Add(const Matrix& mat, float scalar)
    {
        Matrix result(mat);
        result.M[0] += scalar;
        result.M[1] += scalar;
        result.M[2] += scalar;
        result.M[3] += scalar;
        return result;
    }

    Matrix Matrix::Sub(const Matrix& mat0, const Matrix& mat1)
    {
        Matrix result(mat0);
        result.M[0] -= mat1.M[0];
        result.M[1] -= mat1.M[1];
        result.M[2] -= mat1.M[2];
        result.M[3] -= mat1.M[3];
        return result;
    }

    Matrix Matrix::Sub(const Matrix& mat, float scalar)
    {
        Matrix result(mat);
        result.M[0] -= scalar;
        result.M[1] -= scalar;
        result.M[2] -= scalar;
        result.M[3] -= scalar;
        return result;
    }

    Matrix Matrix::Mul(const Matrix& mat0, const Matrix& mat1)
    {
        Matrix result;
        // Column 1
        result.M[0] = mat0[0] * mat1[0].X;
        result.M[0] += mat0[1] * mat1[0].Y;
        result.M[0] += mat0[2] * mat1[0].Z;
        result.M[0] += mat0[3] * mat1[0].W;

        result.M[1] = mat0[0] * mat1[1].X;
        result.M[1] += mat0[1] * mat1[1].Y;
        result.M[1] += mat0[2] * mat1[1].Z;
        result.M[1] += mat0[3] * mat1[1].W;

        result.M[2] = mat0[0] * mat1[2].X;
        result.M[2] += mat0[1] * mat1[2].Y;
        result.M[2] += mat0[2] * mat1[2].Z;
        result.M[2] += mat0[3] * mat1[2].W;

        result.M[3] = mat0[0] * mat1[3].X;
        result.M[3] += mat0[1] * mat1[3].Y;
        result.M[3] += mat0[2] * mat1[3].Z;
        result.M[3] += mat0[3] * mat1[3].W;
        return result;
    }

    Matrix Matrix::Mul(const Matrix& mat, float scalar)
    {
        Matrix res(mat);
        res.M[0] *= scalar;
        res.M[1] *= scalar;
        res.M[2] *= scalar;
        res.M[3] *= scalar;
        return res;
    }

    Matrix Matrix::Div(const Matrix& mat0, const Matrix& mat1)
    {
        Matrix res(mat0);
        res.M[0] /= mat1.M[0];
        res.M[1] /= mat1.M[1];
        res.M[2] /= mat1.M[2];
        res.M[3] /= mat1.M[3];
        return res;
    }

    Matrix Matrix::Div(const Matrix& mat, float scalar)
    {
        Matrix res(mat);
        res.M[0] /= scalar;
        res.M[1] /= scalar;
        res.M[2] /= scalar;
        res.M[3] /= scalar;
        return res;
    }

    Matrix Matrix::Inverse(const Matrix& mat)
    {
        float coef00 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
        float coef02 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];
        float coef03 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];

        float coef04 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
        float coef06 = mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3];
        float coef07 = mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3];

        float coef08 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
        float coef10 = mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2];
        float coef11 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];

        float coef12 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
        float coef14 = mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3];
        float coef15 = mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3];

        float coef16 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
        float coef18 = mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2];
        float coef19 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];

        float coef20 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
        float coef22 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];
        float coef23 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

        Vector4 fac0(coef00, coef00, coef02, coef03);
        Vector4 fac1(coef04, coef04, coef06, coef07);
        Vector4 fac2(coef08, coef08, coef10, coef11);
        Vector4 fac3(coef12, coef12, coef14, coef15);
        Vector4 fac4(coef16, coef16, coef18, coef19);
        Vector4 fac5(coef20, coef20, coef22, coef23);

        Vector4 vec0(mat[1][0], mat[0][0], mat[0][0], mat[0][0]);
        Vector4 vec1(mat[1][1], mat[0][1], mat[0][1], mat[0][1]);
        Vector4 vec2(mat[1][2], mat[0][2], mat[0][2], mat[0][2]);
        Vector4 vec3(mat[1][3], mat[0][3], mat[0][3], mat[0][3]);

        Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
        Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
        Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
        Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

        Vector4 signA(+1, -1, +1, -1);
        Vector4 signB(-1, +1, -1, +1);
        Matrix inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

        Vector4 row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

        Vector4 dot0(mat[0] * row0);
        float dot1 = (dot0.X + dot0.Y) + (dot0.Z + dot0.W);

        float oneOverDeterminant = static_cast<float>(1) / dot1;
        return inverse * oneOverDeterminant;
    }

    Matrix Matrix::Transpose(const Matrix& mat)
    {
        Matrix res;
        res[0][0] = mat[0][0];
        res[0][1] = mat[1][0];
        res[0][2] = mat[2][0];
        res[0][3] = mat[3][0];

        res[1][0] = mat[0][1];
        res[1][1] = mat[1][1];
        res[1][2] = mat[2][1];
        res[1][3] = mat[3][1];

        res[2][0] = mat[0][2];
        res[2][1] = mat[1][2];
        res[2][2] = mat[2][2];
        res[2][3] = mat[3][2];

        res[3][0] = mat[0][3];
        res[3][1] = mat[1][3];
        res[3][2] = mat[2][3];
        res[3][3] = mat[3][3];
        return res;
    }

    Matrix Matrix::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
    {
        Vector3 f((center - eye).Normalized());
        Vector3 s((f.Cross(up)).Normalized());
        Vector3 u(s.Cross(f));

        Matrix res(Identity);
        res[0][0] = s.X;
        res[1][0] = s.Y;
        res[2][0] = s.Z;
        res[0][1] = u.X;
        res[1][1] = u.Y;
        res[2][1] = u.Z;
        res[0][2] = -f.X;
        res[1][2] = -f.Y;
        res[2][2] = -f.Z;
        res[3][0] = -s.Dot(eye);
        res[3][1] = -u.Dot(eye);
        res[3][2] = f.Dot(eye);
        return res;
    }

    Matrix Matrix::Perspective(float fovy, float aspect, float zNear, float zFar)
    {
        Matrix res;
        float tanHalfFovy = Math::Tan(fovy / 2.0f);

        res[0][0] = 1 / (aspect * tanHalfFovy);
        res[1][1] = 1 / tanHalfFovy;
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -1;
        res[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
        return res;
    }

    Matrix Matrix::Orthographic(float left, float right, float bottom, float top)
    {
        Matrix res;
        res[0][0] = 2.0f / (right - left);
        res[1][1] = 2.0f / (top - bottom);
        res[2][2] = -2.0f;
        res[3][0] = -(right + left) / (right - left);
        res[3][1] = -(top + bottom) / (top - bottom);
        return res;
    }

    Matrix Matrix::Translate(const Matrix& mat, const Vector3& translation)
    {
        Matrix res(mat);
        res[3] = translation;
        return res;
    }

    Matrix Matrix::Scale(const Matrix& mat, const Vector3& scale)
    {
        Matrix res;
        res[0] = mat.M[0] * scale.X;
        res[1] = mat.M[1] * scale.Y;
        res[2] = mat.M[2] * scale.Z;
        res[3] = mat.M[3];
        return res;
    }

    Matrix Matrix::Rotate(const Matrix& mat, float angle, const Vector3& rotate)
    {
        float const a = angle;
        float const c = Math::Cos(a);
        float const s = Math::Sin(a);

        Vector3 axis(rotate.Normalized());
        Vector3 temp((1.0f - c) * axis);

        Matrix rot;
        rot[0][0] = c + temp.X * axis.X;
        rot[0][1] = temp.X * axis.Y + s * axis.Z;
        rot[0][2] = temp.X * axis.Z - s * axis.Y;

        rot[1][0] = temp.Y * axis.X - s * axis.Z;
        rot[1][1] = c + temp.Y * axis.Y;
        rot[1][2] = temp.Y * axis.Z + s * axis.X;

        rot[2][0] = temp.Z * axis.X + s * axis.Y;
        rot[2][1] = temp.Z * axis.Y - s * axis.X;
        rot[2][2] = c + temp.Z * axis.Z;

        Matrix res;
        res[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
        res[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
        res[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
        res[3] = mat[3];
        return res;
    }

    bool Formatter<Matrix>::Parse(const FormatArgsContext& context)
    {
        return true;
    }

    void Formatter<Matrix>::FormatTo(String& dest, const Matrix& mat)
    {
        Formatter<Vector4> formatter;
        formatter.FormatTo(dest, mat.M[0]);
        dest.Append('\n');
        formatter.FormatTo(dest, mat.M[1]);
        dest.Append('\n');
        formatter.FormatTo(dest, mat.M[2]);
        dest.Append('\n');
        formatter.FormatTo(dest, mat.M[3]);
    }

    String ToString(const Matrix& mat)
    {
        return Format("{}\n{}\n{}\n{}\n", mat.M[0], mat.M[1], mat.M[2], mat.M[3]);
    }
}
