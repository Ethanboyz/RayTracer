#include "rt/math/vec3.hpp"
#include "rt/utilities.hpp"

uvec3 scatter_uvec3(const uvec3& normal) {
    // Random point on a unit disk converted to polar coords distributed uniformly by area
    const float r{std::sqrt(Utilities::random_float())};
    const float phi{2.f * static_cast<float>(M_PI) * Utilities::random_float()};
    const float x{r * std::cos(phi)};
    const float y{r * std::sin(phi)};
    const float z{std::sqrt(std::max(0.f, 1.f - r * r))};

    // Orthonormal basis forms temp coord frame at the hit point
    const uvec3 w{normal};
    const uvec3 a{(std::fabs(w.x()) > 0.9f) ? uvec3{0,1,0} : uvec3{1,0,0}};
    const uvec3 v{unit(cross(w, a))};
    const uvec3 u{unit(cross(v, w))};

    return unit(u*x + v*y + w*z);
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
