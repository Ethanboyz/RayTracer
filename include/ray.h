#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() = default;
    Ray(const Coord3& pos, const Vec3& dir) : orig{pos}, dir{dir} {}

    // Keep rays immutable
    const constexpr Coord3& origin() const noexcept { return orig; }
    const constexpr Vec3& direction() const noexcept { return dir; }

    Coord3 pos(const float t) const noexcept { return Coord3{orig + t * dir}; }

private:
    Coord3 orig;
    Vec3 dir;
};

#endif