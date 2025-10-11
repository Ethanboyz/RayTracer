#include "rt/math/vec3.hpp"

uvec3 scatter_uvec3(const uvec3& normal) {
    vec3 p;
    while (true) {
        p = {Utilities::random_float({-1, 1}), Utilities::random_float({-1, 1}), Utilities::random_float({-1, 1})};
        if (dot(p, p) < 1.f) {
            break;
        }
    }
    vec3 result{p + nounit(normal)};
    if (result.degenerate()) {
        result = nounit(normal);
    }
    return unit(result);
}

uvec3 reflect_uvec3(const uvec3& v, const uvec3& normal) {
    // Direction = v + 2(projection of v on normal)
    return unit(nounit(v) - 2 * dot(v, normal) * normal);
}

uvec3 refract_uvec3(const uvec3& v, const uvec3& normal, const float eta, const float eta_prime) {
    const float cos_theta{std::fmin(-dot(normal, v), 1.f)};
    const float eta_ratio{eta / eta_prime};
    const float k{1 - eta_ratio * eta_ratio * (1 - cos_theta * cos_theta)};

    // Schlick's approximation setup
    const float r0{(eta - eta_prime) / (eta + eta_prime) * ((eta - eta_prime) / (eta + eta_prime))};
    const float reflectance{r0 + (1 - r0) * std::pow(1 - cos_theta, 5.f)};

    // Total internal reflection or specular reflection from Schlick's approximation
    if (k < 0.f || reflectance > Utilities::random_float()) {
        return reflect_uvec3(v, normal);
    }
    const float sqrt_k = std::sqrt(std::max(0.f, k));
    return unit(eta_ratio * v + (eta_ratio * cos_theta - sqrt_k) * normal);
}
