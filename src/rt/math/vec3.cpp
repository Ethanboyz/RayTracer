#include "rt/math/vec3.hpp"

uvec3 scatter_uvec3(const uvec3& normal) {
    // Direction = random unit vector in the direction of the outward-facing normal (make sure it's not all near-zeros)
    const uvec3 scattered_direction{Interval{0.f, 1.f}};

    // Add by normal to ensure the scattered direction faces outward, randomized direction follows cos() distribution
    return unit(scattered_direction + normal);
}

uvec3 reflect_uvec3(const uvec3& v, const uvec3& normal) {
    // Direction = v + 2(projection of v on normal)
    return unit(nounit(v) - 2 * dot(v, normal) * normal);
}

uvec3 refract_uvec3(const uvec3& v, const uvec3& normal, const float eta, const float eta_prime) {
    const float cos_theta{std::fmin(-dot(normal, v), 1.f)};
    const float eta_ratio{eta / eta_prime};
    const float k{1 - eta_ratio * eta_ratio * (1 - cos_theta * cos_theta)};

    if (k < 0.f) {
        // Total internal reflection
        return reflect_uvec3(v, normal);
    }
    const float sqrt_k = std::sqrt(std::max(0.f, k));
    return unit(eta_ratio * v + (eta_ratio * cos_theta - sqrt_k) * normal);
}

