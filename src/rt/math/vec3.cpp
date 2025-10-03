#include "rt/math/vec3.hpp"

uvec3 scatter_uvec3(const uvec3& normal) {
    // Direction = random unit vector in the direction of the outward-facing normal (make sure it's not all near-zeros)
    const uvec3 scattered_direction{Interval{0.f, 1.f}};

    // Add by normal to ensure the scattered direction faces outward, randomized direction follows cos() distribution
    return unit(scattered_direction + normal);
}

uvec3 reflect_uvec3(const vec3& v, const uvec3& normal) {
    // Direction = v + 2(projection of v on normal)
    return unit(v - 2 * dot(v, normal) * normal);
}