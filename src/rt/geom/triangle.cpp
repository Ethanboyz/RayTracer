#include "rt/geom/triangle.hpp"
#include "rt/math/ray.hpp"

bool Triangle::ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const {
    const vec3 ab{b_ - a_};
    const vec3 ac{c_ - a_};
    const vec3 ray_cross_ac{cross(nounit(ray.direction()), ac)};
    const float det{dot(ab, ray_cross_ac)};

    // Ray parallel to triangle = never intersects
    constexpr float epsilon{1e-6};
    if (std::fabs(det) < epsilon) {
        return false;
    }

    const float inv_det{1.f / det};
    const vec3 r{ray.origin() - a_};

    const float u{inv_det * dot(r, ray_cross_ac)};
    if (constexpr Interval interval{0.f, 1.f}; !interval.inclusive_contains(u, epsilon)) {
        return false;
    }
    const vec3 r_cross_ab{cross(r, ab)};
    if (const float v{inv_det * dot(ray.direction(), r_cross_ab)}; v < -epsilon || u + v > 1 + epsilon) {
        return false;
    }

    const float ray_t{inv_det * dot(ac, r_cross_ab)};
    if (!t.inclusive_contains(ray_t, epsilon)) {
        return false;
    }
    // Initialize all HitRecord fields before returning
    hit_record.point(ray.position(ray_t));
    hit_record.t(ray_t);
    hit_record.set_face_normal(ray, unit(cross(ab, ac)));
    hit_record.material(material_);
    return true;
}