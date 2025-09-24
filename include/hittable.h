#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "interval.h"

// More to be added in the future
class HitRecord {
public:
    HitRecord() : ray_t{0}, frnt_face{true} {}
    HitRecord(
        const coord3& point,
        const uvec3& normal,
        const float t,
        const bool front_face,
        const Color& color
    ) :
        p{point},
        n{normal},
        ray_t{t},
        frnt_face{front_face},
        col{color} {}

    // Accessors
    constexpr coord3 point() const noexcept { return p; }
    constexpr uvec3 normal() const noexcept { return n; }
    constexpr float t() const noexcept { return ray_t; }
    constexpr float front_face() const noexcept { return frnt_face; }
    constexpr Color color() const noexcept { return col; }

    constexpr void point(const coord3 point) noexcept { p = point; }
    constexpr void normal(const uvec3 normal) noexcept { n = normal; }
    constexpr void t(const float t) noexcept { ray_t = t; }
    constexpr void front_face(const bool front_face) noexcept { frnt_face = front_face; }
    constexpr void color(const Color color) noexcept { col = color; }

    bool set_face_normal(const Ray& r, const uvec3& outward_normal) {
        if (dot(r.direction(), n) > 0.f) {
            // Ray is inside sphere, surface is facing away, flip normal
            n = -outward_normal;
            frnt_face = false;
            return false;
        } else {
            // Ray is outside sphere, surface is facing towards camera
            n = outward_normal;
            frnt_face = true;
            return true;
        }
    }

private:
    coord3 p;           // Coordinates of hit
    uvec3 n;            // Normal of hit surface (never faces against direction of generated rays)
    float ray_t;        // t-value of the ray where hit occurred
    bool frnt_face;     // True if surface is facing towards the camera
    Color col;          // Color to be drawn
};

// Any sort of renderable object should be Hittable
class Hittable {
public:
    virtual ~Hittable() = default;

    // If ray intersects, populates hit_record with Ray-Hittable intersect info
    virtual bool ray_hit(const Ray& r, const Interval<float>& t, HitRecord& hit_record) const = 0;
};

#endif