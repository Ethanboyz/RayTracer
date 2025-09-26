#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"

// More to be added in the future
class HitRecord {
public:
    constexpr HitRecord() : ray_t{0}, frnt_face{true}, mat{{0, 0, 0}, 0.f, 0.f}, light{0} {}
    HitRecord(
        const coord3& point,
        const uvec3& normal,
        const float t,
        const bool front_face,
        const Material& material
    ) :
        p{point},
        n{normal},
        ray_t{t},
        frnt_face{front_face},
        mat{material},
        light{0} {}

    // Accessors
    constexpr coord3 point() const noexcept { return p; }
    constexpr uvec3 normal() const noexcept { return n; }
    constexpr float t() const noexcept { return ray_t; }
    constexpr bool front_face() const noexcept { return frnt_face; }            // True if surface is facing camera
    constexpr Material material() const noexcept { return mat; }
    constexpr float light_intensity() const noexcept { return light; }

    constexpr void point(const coord3 point) noexcept { p = point; }
    constexpr void normal(const uvec3 normal) noexcept { n = normal; }
    constexpr void t(const float t) noexcept { ray_t = t; }
    constexpr void front_face(const bool front_face) noexcept { frnt_face = front_face; }
    constexpr void material(const Material& material) noexcept { mat = material; }
    constexpr void light_intensity(const float light_intensity) noexcept { light = light_intensity; }

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
    coord3 p;               // Coordinates of hit
    uvec3 n;                // Normal of hit surface (never points away from the camera)
    float ray_t;            // t-value of the ray where hit occurred
    bool frnt_face;         // True if surface is facing towards the camera
    Material mat;           // Determines the reflective and color properties to be drawn
    float light;            // Intensity of light exposure (ranges from 0-1, 0.5 means the base color is drawn)
};

// Any sort of renderable object should be Hittable
class Hittable {
public:
    virtual ~Hittable() = default;

    // If ray intersects, populates hit_record with Ray-Hittable intersect info
    virtual bool ray_hit(const Ray& r, const Interval<float>& t, HitRecord& hit_record) const = 0;
};

#endif