#include "rt/scene/material.hpp"
#include "rt/geom/hittable.hpp"
#include "rt/math/ray.hpp"

// With probability reflectance_, create a new scattered child ray in random direction or a new reflected ray
bool Material::scatter(const Ray &ray, const HitRecord &hit_record, Color &attenuation, Ray &scattered) const {
    if (Utilities::random_float() > reflectance_) {
        return false;
    }

    // Specular reflecting
    if (Utilities::random_float() <= shininess_) {
        scattered = Ray{hit_record.point(), reflect_uvec3(nounit(ray.direction()), hit_record.normal())};
        attenuation = albedo_;
    } else {
        // Diffuse scattering
        scattered = Ray{hit_record.point(), random_uvec3(hit_record.normal())};
        attenuation = albedo_;
    }
    return true;
}
