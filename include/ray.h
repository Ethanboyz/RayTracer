#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() = default;
    Ray(const coord3& position, const uvec3& direction) : orig{position}, dir{direction} {}

    // Keep rays immutable
    const constexpr coord3& origin() const noexcept { return orig; }
    const constexpr uvec3& direction() const noexcept { return dir; }

    coord3 position(const float t) const noexcept { return coord3{orig + t * dir}; }

private:
    coord3 orig;
    uvec3 dir;
};

#endif