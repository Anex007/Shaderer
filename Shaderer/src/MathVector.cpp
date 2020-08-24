#include "MathVector.h"

Vec3f operator *(const Vec3f& v1, float m)
{
    Vec3f res(v1.x * m, v1.y * m, v1.z * m);
    return res;
}
