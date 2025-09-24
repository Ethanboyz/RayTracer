#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    Sphere(const coord3& center, const float radius) : center_pos{center}, rad{radius} {}

    // Accessors
    constexpr coord3 position() const noexcept { return center_pos; }
    constexpr float radius() const noexcept { return rad; }

    // Returns true and populates hit_record if sphere intersects with ray (min_t <= t <= max_t)
    bool ray_hit(const Ray& r, const Interval<float>& t, HitRecord& hit_record) const override {
        const float a = {r.direction().length_squared()};
        const float b = {dot(nounit(r.direction()), center_pos - r.origin())};
        const float c = {(center_pos - r.origin()).length_squared() - (rad * rad)};
        const float det = {(b * b) - a * c};
        if (det < 0) {
            return false;
        }
        float calculated_t = (b - std::sqrt(det)) / a;
        if (calculated_t < t.min() || calculated_t > t.max()) {
            calculated_t = (b + std::sqrt(det)) / a;       // Still the quadratic formula, eval other root
            if (calculated_t < t.min() || calculated_t > t.max()) {
                return false;
            }
        }

        // Initialize all HitRecord fields before returning
        hit_record.point(r.position(calculated_t));
        hit_record.t(calculated_t);
        hit_record.set_face_normal(r, unit(hit_record.point() - center_pos));
        return true;
    }

private:
    coord3 center_pos;
    float rad;
};

#endif