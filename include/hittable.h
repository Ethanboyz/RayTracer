#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

// More to be added in the future
class HitRecord {
public:
    HitRecord() : ray_t{0}, frnt_face{true} {}
    HitRecord(
        const Coord3& point,
        const Vec3& normal,
        const float t,
        const bool front_face
    ) :
        p{point},
        n{unit(normal)},
        ray_t{t},
        frnt_face{front_face} {}

    // Accessors
    constexpr Coord3 point() const noexcept { return p; }
    constexpr Vec3 normal() const noexcept { return n; }
    constexpr float t() const noexcept { return ray_t; }
    constexpr float front_face() const noexcept { return frnt_face; }

    constexpr void point(const Coord3 point) noexcept { p = point; }
    void normal(const Vec3 normal) noexcept { n = unit(normal); }
    constexpr void t(const float t) noexcept { ray_t = t; }
    constexpr void front_face(const bool front_face) noexcept { frnt_face = front_face; }

    bool set_face_normal(const Ray& r) {
        if (dot(r.direction(), n) > 0.f) {
            // Ray is inside sphere, surface is facing away, flip normal
            n = -n;
            frnt_face = false;
            return false;
        } else {
            // Ray is outside sphere, surface is facing towards camera
            frnt_face = true;
            return true;
        }
    }

private:
    Coord3 p;
    Vec3 n;
    float ray_t;
    bool frnt_face;
};

// Any sort of renderable object should be Hittable
class Hittable {
public:
    virtual ~Hittable() = default;

    // If ray intersects, populates hit_record with Ray-Hittable intersect info
    virtual bool ray_hit(const Ray& r, float min_t, float max_t, HitRecord& hit_record) const = 0;
};

#endif