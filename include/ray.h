#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const coord3& pos, const vec3& dir) : orig{pos}, dir{dir} {}

    // Keep rays immutable
    const constexpr coord3& origin() const noexcept { return orig; }
    const constexpr vec3& direction() const noexcept { return dir; }

    coord3 pos(const float t) const noexcept { return coord3{orig + t * dir}; }

private:
    coord3 orig;
    vec3 dir;
};

#endif