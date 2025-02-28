#pragma once

#include <cmath>

#include "Oniun/Core/BaseTypes.h"

#define PI 3.1415926535897932f
#define PI2 6.28318530718f
#define RAD_TO_DEG 57.295779513082320876798154814105f
#define DEG_TO_RAD 0.01745329251994329576923690768489f

namespace Math
{
    FORCE_INLINE float Radians(float degrees)
    {
        return degrees * DEG_TO_RAD;
    }

    FORCE_INLINE float Degrees(float radians)
    {
        return radians * RAD_TO_DEG;
    }

    /// Round up
    FORCE_INLINE float Trunc(float val)
    {
        return truncf(val);
    }

    // Round to nearest integer
    FORCE_INLINE float Round(float val)
    {
        return roundf(val);
    }

    // Round down
    FORCE_INLINE float Floor(float val)
    {
        return floorf(val);
    }

    // Round up to the nearest integer that is greater than or equal to the number
    FORCE_INLINE float Ceil(float val)
    {
        return ceilf(val);
    }

    FORCE_INLINE float Sin(float val)
    {
        return sinf(val);
    }

    FORCE_INLINE float Asin(float val)
    {
        return asinf(val < -1.f ? -1.f : val < 1.f ? val : 1.f);
    }

    FORCE_INLINE float Sinh(float val)
    {
        return sinhf(val);
    }

    FORCE_INLINE float Cos(float val)
    {
        return cosf(val);
    }

    FORCE_INLINE float Acos(float val)
    {
        return acosf(val < -1.f ? -1.f : val < 1.f ? val : 1.f);
    }

    FORCE_INLINE float Tan(float val)
    {
        return tanf(val);
    }

    FORCE_INLINE float Atan(float val)
    {
        return atanf(val);
    }

    FORCE_INLINE float Atan2(float y, float x)
    {
        return atan2f(y, x);
    }

    FORCE_INLINE float Sqrt(const float val)
    {
        return sqrtf(val);
    }

    FORCE_INLINE float InvSqrt(float val)
    {
        return 1.0f / sqrtf(val);
    }

    FORCE_INLINE float Abs(const float val)
    {
        return fabsf(val);
    }

    FORCE_INLINE int32 Abs(const int32 val)
    {
        return val < 0 ? -val : val;
    }

    FORCE_INLINE int64 Abs(const int64 val)
    {
        return val < 0 ? -val : val;
    }

    FORCE_INLINE float Log(const float val)
    {
        return logf(val);
    }

    FORCE_INLINE float Log2(const float val)
    {
        return log2f(val);
    }

    FORCE_INLINE float Log10(const float val)
    {
        return log10f(val);
    }

    FORCE_INLINE float Pow(const float base, const float exponent)
    {
        return powf(base, exponent);
    }

    FORCE_INLINE float Exp(const float val)
    {
        return expf(val);
    }

    FORCE_INLINE float Exp2(const float val)
    {
        return exp2f(val);
    }

    FORCE_INLINE int32 Mod(const int32 a, const int32 b)
    {
        return (int32)fmodf((float)a, (float)b);
    }

    FORCE_INLINE float Mod(const float a, const float b)
    {
        return fmodf(a, b);
    }

    // Break a floating-point number into its integer and fractional parts.
    FORCE_INLINE float ModF(const float a, float* b)
    {
        return modff(a, b);
    }

    FORCE_INLINE float Frac(float val)
    {
        return val - Floor(val);
    }

    template<typename T>
    FORCE_INLINE bool InRange(T val, T min, T max)
    {
        return val > min && val < max;
    }

    template<typename T>
    FORCE_INLINE T Clamp(const T& val, const T& min, const T& max)
    {
        return val < min ? min : val > max ? max : val;
    }

    template<typename T>
    FORCE_INLINE T Min(const T& v0, const T& v1)
    {
        return v0 < v1 ? v0 : v1;
    }

    template<class T>
    FORCE_INLINE T Min(const T& v0, const T& v1, const T& v2)
    {
        return Min(Min(v0, v1), v2);
    }

    template<class T>
    FORCE_INLINE T Min(const T& v0, const T& v1, const T& v2, const T& v3)
    {
        return Min(Min(Min(v0, v1), v2), v3);
    }

    template<typename T>
    FORCE_INLINE T Max(const T& v0, const T& v1)
    {
        return v0 > v1 ? v0 : v1;
    }

    template<class T>
    FORCE_INLINE T Max(const T& v0, const T& v1, const T& v2)
    {
        return Max(Max(v0, v1), v2);
    }

    template<class T>
    FORCE_INLINE T Max(const T& v0, const T& v1, const T& v2, const T& v3)
    {
        return Max(Max(Max(v0, v1), v2), v3);
    }

    template<typename T>
    FORCE_INLINE T Lerp(const T& v0, const T& v1, const T& t)
    {
        return (1.0f - t) * v0 + t * v1;
    }

    template<typename T>
    FORCE_INLINE T InverseLerp(const T& v0, const T& v1, const T& t)
    {
        if (v0 == v1)
            return 0;
        return Clamp((t - v0) / (v1 - v0), 0, 1);
    }

    template<typename T>
    FORCE_INLINE T Wrap(const T& t, const T& max)
    {
        return t - Floor(t / max) * max;
    }

    template<typename T>
    FORCE_INLINE T IsNaN(T val)
    {
        return isnan(val);
    }
}
