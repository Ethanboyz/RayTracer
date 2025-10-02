#include "rt/geom/sphere.hpp"
#include "rt/math/ray.hpp"

bool Sphere::ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const {
    const float a{ray.direction().length_squared()};
    const float b{dot(nounit(ray.direction()), center_ - ray.origin())};
    const float c{(center_ - ray.origin()).length_squared() - radius_ * radius_};
    const float det{b * b - a * c};
    if (det < 0) {
        return false;
    }

    // Up to 2 possible intersections, pick the one with smallest t-value
    float calculated_t{(b - std::sqrt(det)) / a};
    if (calculated_t < t.min() || calculated_t > t.max()) {
        calculated_t = (b + std::sqrt(det)) / a;
        if (calculated_t < t.min() || calculated_t > t.max()) {
            return false;
        }
    }

    // Initialize all HitRecord fields before returning (light_intensity is set elsewhere)
    hit_record.point(ray.position(calculated_t));
    hit_record.t(calculated_t);
    hit_record.set_face_normal(ray, unit(hit_record.point() - center_));
    hit_record.material(material_);
    return true;
}