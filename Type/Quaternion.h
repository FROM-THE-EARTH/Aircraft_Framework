#pragma once

#include "Vec3.h"

constexpr float PI = 3.141592654f;

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

    float length()
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    Quaternion normalized()
    {
        return {x / length(), y / length(), z / length(), w / length()};
    }

    float toAngle()
    {
        Quaternion q(1, 0, 0, 0);
        q = *this * q * conjugate();
        return acos(sqrt(q.x * q.x + q.y * q.y) / sqrt(q.x * q.x + q.y * q.y + q.z * q.z)) * 180 / PI;
    }

    /*=======================method================================*/

    //return the quaternion angular velocity applied
    Quaternion angularVelocityApplied(Vec3 v);

    //return the conjugate quaternion
    Quaternion conjugate() const
    {
        return {
            -x, -y, -z, w};
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
};