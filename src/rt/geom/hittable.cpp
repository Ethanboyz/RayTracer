#include "rt/geom/hittable.hpp"

constexpr HitRecord::HitRecord(const coord3& point, const uvec3& normal, const float t, const bool front_face, const Material& material)
  : point_{point},
    normal_{normal},
    ray_t_{t},
    front_face_{front_face},
    material_{material},
    light_intensity_{0} {}

bool HitRecord::set_face_normal(const Ray& ray, const uvec3& normal) {
    normal_ = normal;
    const float r_dot_n{dot(ray.direction(), normal)};

    // Surface faces the camera when surface normal direction opposes ray direction
    front_face_ = r_dot_n <= 0.f;
    return r_dot_n <= 0.f;
}