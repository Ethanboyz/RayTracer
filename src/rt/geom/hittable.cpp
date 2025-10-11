#include "rt/geom/hittable.hpp"
#include "rt/math/ray.hpp"

constexpr HitRecord::HitRecord(const coord3& point, const uvec3& normal, const float t, const bool front_face, const Material& material)
  : point_{point},
    normal_{normal},
    ray_t_{t},
    front_face_{front_face},
    material_{material} {}

bool HitRecord::set_face_normal(const Ray& ray, const uvec3& normal) {
    normal_ = normal;
    const float r_dot_n{dot(ray.direction(), normal)};

    // Surface faces the camera when surface normal direction opposes ray direction
    front_face_ = r_dot_n <= 0.f;
    return r_dot_n <= 0.f;
}

bool HitRecord::bounce(const Ray& ray, Color &attenuation, Ray &next) const {
    const float random{Utilities::random_float()};
    uvec3 n{normal_};
    if (!front_face_) {
        n = -n;
    }

    // With probability reflectance, create a new scattered or reflected ray
    if (random <= material_.reflectance()) {
        if (Utilities::random_float() <= material_.shininess()) {
            // Specular reflecting
            next = Ray{point_, reflect_uvec3(ray.direction(), n)};
            attenuation = material_.albedo();
        } else {
            // Diffuse scattering
            next = Ray{point_, scatter_uvec3(n)};
            attenuation = material_.albedo();
        }
        return true;
    }

    // With probability refracted, create a new refracted ray
    if (random <= material_.reflectance() + material_.refraction()) {
        float first_refractive_index;
        float second_refractive_index;

        // Indices need to be inverted depending on if ray is inside or outside
        if (front_face_) {
            first_refractive_index = 1.f;
            second_refractive_index = material_.refraction_index();
        } else {
            first_refractive_index = material_.refraction_index();
            second_refractive_index = 1.f;
        }
        next = Ray{point_, refract_uvec3(ray.direction(), n, first_refractive_index, second_refractive_index)};
        attenuation = Color{1, 1, 1};
        return true;
    }
    return false;
}

Color HitRecord::emitted() const {
    return material_.albedo() * material_.emittance();
}
