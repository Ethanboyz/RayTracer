#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

// More to be added in the future
class HitRecord {
public:
    HitRecord() : ray_t{0} {}
    HitRecord(const Coord3& point, const Vec3& normal, const float t) : p{point}, n{normal}, ray_t{t} {}

    // Accessors
    constexpr Coord3 point() const noexcept { return p; }
    constexpr Vec3 normal() const noexcept { return n; }
    constexpr float t() const noexcept { return ray_t; }

    constexpr void point(const Coord3 point) noexcept { p = point; }
    constexpr void normal(const Vec3 normal) noexcept { n = normal; }
    constexpr void t(const float t) noexcept { ray_t = t; }

private:
    Coord3 p;
    Vec3 n;
    float ray_t;
};

// Any sort of renderable object should be Hittable
class Hittable {
public:
    virtual ~Hittable() = default;

    // If ray intersects, populates hit_record with Ray-Hittable intersect info
    virtual bool ray_hit(const Ray& r, float min_t, float max_t, HitRecord& hit_record) const = 0;
};

#endif