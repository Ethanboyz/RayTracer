#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"

/**
 * @class HitRecord
 * @brief Stores the characteristics of a ray-object hit location.
 */
class HitRecord {
public:
    /**
     * @brief Constructs a default HitRecord (should be reassigned).
     */
    constexpr HitRecord() : ray_t_{0}, front_face_{true}, material_{{0, 0, 0}, 0.f, 0.f}, light_intensity_{0} {}

    /**
     * @brief Constructs a HitRecord that stores the information of a ray hit.
     * @param point Coordinates of the hit location.
     * @param normal Normal vector of the hit surface (currently always points towards the camera's direction, will change this to always outwards facing).
     * @param t t-value where the ray hit the surface.
     * @param front_face True if the surface's outward-facing face is towards the camera.
     * @param material Material of the hit surface.
     */
    constexpr HitRecord(
        const coord3& point,
        const uvec3& normal,
        const float t,
        const bool front_face,
        const Material& material
    ) :
        point_{point},
        normal_{normal},
        ray_t_{t},
        front_face_{front_face},
        material_{material},
        light_intensity_{0} {}

    // Accessors
    /** @return Coordinates of the hit location. */
    [[nodiscard]] constexpr coord3 point() const noexcept { return point_; }
    /** @return Normal vector of the hit surface. */
    [[nodiscard]] constexpr uvec3 normal() const noexcept { return normal_; }
    /** @return t-value where the ray hit the surface. */
    [[nodiscard]] constexpr float t() const noexcept { return ray_t_; }
    /** @return True if the surface's outward-facing face is towards the camera. */
    [[nodiscard]] constexpr bool front_face() const noexcept { return front_face_; }
    /** @return Material of the hit surface. */
    [[nodiscard]] constexpr Material material() const noexcept { return material_; }
    /** @return Light intensity of the hit location. */
    [[nodiscard]] constexpr float light_intensity() const noexcept { return light_intensity_; }

    /** @param point Sets hit location coordinates. */
    constexpr void point(const coord3 point) noexcept { point_ = point; }
    /** @param normal Sets normal vector. */
    constexpr void normal(const uvec3 normal) noexcept { normal_ = normal; }
    /** @param t Sets the t-value of ray hit. */
    constexpr void t(const float t) noexcept { ray_t_ = t; }
    /** @param front_face Set to true if the surface faces towards the camera. */
    constexpr void front_face(const bool front_face) noexcept { front_face_ = front_face; }
    /** @param material Sets the material of the hit surface. */
    constexpr void material(const Material& material) noexcept { material_ = material; }
    /** @param light_intensity Sets the light intensity at the hit location. */
    constexpr void light_intensity(const float light_intensity) noexcept { light_intensity_ = light_intensity; }

    /**
     * @brief Sets the normal of the HitRecord based on if the hit surface is outward-facing.
     * @param ray Generated ray from the camera.
     * @param outward_normal Outward-facing normal of the hit surface.
     * @return True if the hit surface is facing towards the camera, false otherwise.
     */
    bool set_face_normal(const Ray& ray, const uvec3& outward_normal) {
        if (dot(ray.direction(), normal_) > 0.f) {
            // Ray is inside sphere, surface is facing away, flip normal
            normal_ = -outward_normal;
            front_face_ = false;
            return false;
        }
        // Ray is outside sphere, surface is facing towards camera
        normal_ = outward_normal;
        front_face_ = true;
        return true;
    }

private:
    coord3 point_;              // Coordinates of hit
    uvec3 normal_;              // Normal of hit surface (never points away from the camera)
    float ray_t_;               // t-value of the ray where hit occurred
    bool front_face_;           // True if surface is facing towards the camera
    Material material_;         // Determines the reflective and color properties to be drawn
    float light_intensity_;     // Intensity of light exposure (ranges from 0-1, 1 means the base color is drawn)
};

/**
 * @class Hittable
 * @brief Defines the properties of any object that can intersect a ray upon rendering.
 */
class Hittable {
public:
    virtual ~Hittable() = default;

    /**
     * @brief Populates hit_record with Ray-Hittable intersect info if ray intersects the current object.
     * @param ray Checked for intersections with the current Hittable object.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @param hit_record Updated with hit information of smallest t if ray intersection occurs.
     * @return True if ray intersects the current object, false otherwise.
     */
    virtual bool ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const = 0;
};

#endif