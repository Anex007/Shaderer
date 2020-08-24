#pragma once

#include <cmath>

// namespace Meth {

    struct Vec2f
    {
        Vec2f(float _v)
            :x(_v), y(_v)
        {}

        Vec2f(float _x, float  _y)
            : x(_x), y(_y)
        {}

        float x, y;
    };

    struct Vec3f
    {
        Vec3f()
            : x(0.0f), y(0.0f), z(0.0f)
        {}

        Vec3f(float _v)
            : x(_v), y(_v), z(_v)
        {}

        Vec3f(float _x, float  _y, float _z)
            : x(_x), y(_y), z(_z)
        {}

        void operator += (const Vec3f& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        }

        void operator -= (const Vec3f& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }

        const Vec3f& Normalize()
        {
            float len = sqrtf(x*x + y*y + z*z);
            x /= len;
            y /= len;
            z /= len;

            return *this;
        }

        /* Does cross product between v1 and v2 and save the result to this. (also normalizes) */
        void Cross(const Vec3f& v1, const Vec3f& v2)
        {
            x = v1.y*v2.z - v1.z*v2.y;
            y = v1.z*v2.x - v1.x*v2.z;
            z = v1.x*v2.y - v1.y*v2.x;

            Normalize();
        }

        /*
        static Vec3f Cross(const Vec3f& v1, const Vec3f& v2)
        {
            Vec3f res;

            res.x = v1.y*v2.z - v1.z*v2.y;
            res.y = v1.z*v2.x - v1.x*v2.z;
            res.z = v1.x*v2.y - v1.y*v2.x;

            return res;
        }
        */

        float x, y, z;
    };

    Vec3f operator *(const Vec3f& v1, float m);

    struct Vec4f
    {
        Vec4f(float _v)
            : x(_v), y(_v), z(_v), w(_v)
        {}

        Vec4f(float _x, float  _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w)
        {}

        float x, y, z, w;
    };

//};
