#include "rt/scene/material.hpp"
#include "rt/geom/hittable.hpp"
#include "rt/math/ray.hpp"

// With probability reflectance_, create a new child ray in random direction or a new reflected ray
bool Material::bounce(const Ray &ray, const HitRecord &hit_record, Color &attenuation, Ray &next) const {
    if (Utilities::random_float() > reflectance_) {
        return false;
    }

    // Specular reflecting
    if (Utilities::random_float() <= shininess_) {
        next = Ray{hit_record.point(), reflect_uvec3(nounit(ray.direction()), hit_record.normal())};
        attenuation = albedo_;
    } else {
        // Diffuse scattering
        next = Ray{hit_record.point(), scatter_uvec3(hit_record.normal())};
        attenuation = albedo_;
    }
    return true;
}
