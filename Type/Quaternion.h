#pragma once

#include "Vec3.h"
#include "MathConstants.h"

struct Quaternion
{

    float x, y, z, w;

    constexpr Quaternion()
        : x(0.0f),
          y(0.0f),
          z(0.0f),
          w(0.0f)
    {
    }

    constexpr Quaternion(float _x, float _y, float _z, float _w)
        : x(_x),
          y(_y),
          z(_z),
          w(_w)
    {
    }

    Quaternion(float direction, float elevation);

    /*========================method=============================*/

    std::string toString() const
    {
        return std::string("(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + ")");
    }

    float length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Quaternion normalized() const
    {
        return {x / length(), y / length(), z / length(), w / length()};
    }

    float toAngle() const
    {
        /*Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
        q = *this * q * conjugate();
        return acos(sqrt(q.x * q.x + q.y * q.y) / sqrt(q.x * q.x + q.y * q.y + q.z * q.z)) * 180.0f / PI;*/

        const float xyz = std::signbit(z) ? -sqrt(x * x + y * y + z * z) : sqrt(x * x + y * y + z * z);

        return acos(sqrt(x * x + y * y) / xyz) * 180.0f / PI;
    }

    /*=======================method================================*/

    //return the quaternion angular velocity applied
    Quaternion angularVelocityApplied(const Vec3 &v);

    //return the conjugate quaternion
    constexpr Quaternion conjugate() const
    {
        return {-x, -y, -z, w};
    }

    /*============================operator=========================*/

    constexpr Quaternion operator*(const Quaternion &q) const
    {
        return {
            w * q.x - z * q.y + y * q.z + x * q.w,
            z * q.x + w * q.y - x * q.z + y * q.w,
            -y * q.x + x * q.y + w * q.z + z * q.w,
            -x * q.x - y * q.y - z * q.z + w * q.w};
    }

    Quaternion &operator+=(const Quaternion &q)
    {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;

        return *this;
    }

    Quaternion &operator*=(const Quaternion &q)
    {
        x = w * q.x - z * q.y + y * q.z + x * q.w;
        y = z * q.x + w * q.y - x * q.z + y * q.w;
        z = -y * q.x + x * q.y + w * q.z + z * q.w;
        w = -x * q.x - y * q.y - z * q.z + w * q.w;

        return *this;
    }

    constexpr Quaternion operator*(float value) const
    {
        return {x * value, y * value, z * value, w * value};
    }

    constexpr Quaternion operator/(float value) const
    {
        return {x / value, y / value, z / value, w / value};
    }
};

inline constexpr Quaternion operator*(float value, const Quaternion &q)
{
    return {value * q.x, value * q.y, value * q.z, value * q.w};
}