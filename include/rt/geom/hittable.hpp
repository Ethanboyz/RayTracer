#ifndef HITTABLE_H
#define HITTABLE_H

#include "rt/scene/material.hpp"
#include "rt/math/vec3.hpp"
class Ray;
class Aabb;

/**
 * @class HitRecord
 * @brief Stores the characteristics of a ray-object hit location.
 */
class HitRecord {
public:
    /** @brief Constructs a default HitRecord (should be reassigned). */
    constexpr HitRecord() : ray_t_{0}, front_face_{true}, material_{Material::create_reflective_material(Color{0, 0, 0}, Reflectance{0}, Shininess{0})} {}

    /**
     * @brief Constructs a HitRecord that stores the information of a ray hit.
     * @param point Coordinates of the hit location.
     * @param normal Outward-facing normal vector of the hit surface.
     * @param t t-value where the ray hit the surface.
     * @param front_face True if the surface's outward-facing face is towards the camera.
     * @param material Material of the hit surface.
     */
    constexpr HitRecord(const coord3& point, const uvec3& normal, float t, bool front_face, const Material& material);

    // Accessors
    /** @return Coordinates of the hit location. */
    [[nodiscard]] constexpr coord3 point() const noexcept { return point_; }
    /** @return Outward-facing normal vector of the hit surface. */
    [[nodiscard]] constexpr uvec3 normal() const noexcept { return normal_; }
    /** @return t-value where the ray hit the surface. */
    [[nodiscard]] constexpr float t() const noexcept { return ray_t_; }
    /** @return True if the surface's outward-facing face is towards the camera. */
    [[nodiscard]] constexpr bool front_face() const noexcept { return front_face_; }
    /** @return Material of the hit surface. */
    [[nodiscard]] constexpr Material material() const noexcept { return material_; }

    /** @param point Sets hit location coordinates. */
    constexpr void point(const coord3 point) noexcept { point_ = point; }
    /** @param normal Sets outward-facing normal vector. */
    constexpr void normal(const uvec3 normal) noexcept { normal_ = normal; }
    /** @param t Sets the t-value of ray hit. */
    constexpr void t(const float t) noexcept { ray_t_ = t; }
    /** @param front_face Set to true if the surface faces towards the camera. */
    constexpr void front_face(const bool front_face) noexcept { front_face_ = front_face; }
    /** @param material Sets the material of the hit surface. */
    constexpr void material(const Material& material) noexcept { material_ = material; }

    /**
     * @brief Sets the normal of the HitRecord and updates front_face state.
     * @param ray Generated ray from the camera.
     * @param normal Outward-facing normal of the hit surface.
     * @return True if the hit surface is facing towards the camera, false otherwise.
     */
    bool set_face_normal(const Ray& ray, const uvec3& normal);

    /**
     * @brief Determines if the current ray hitting the surface should continue as a child ray, outputting
     * the next ray and a color depending on the hit Material.
     *
     * Supports scattering, reflecting, and refraction of rays.
     * @param ray Parent ray hitting the surface.
     * @param attenuation Updated with the attenuation after the hit.
     * @param next Updated with the child ray after this hit.
     * @return True if a child ray was generated as a result of this hit, false if the parent ray was absorbed.
     */
    bool bounce(const Ray& ray, Color& attenuation, Ray& next) const;

private:
    coord3 point_;                          // Coordinates of hit
    uvec3 normal_;                          // Normal of hit surface (outwards-facing)
    float ray_t_;                           // t-value of the ray where hit occurred
    bool front_face_;                       // True if surface is facing towards the camera
    Material material_;                     // Determines the reflective and color properties to be drawn
};

/**
 * @class Hittable
 * @brief Defines the properties of any rendered object that can intersect a ray.
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

    /**
     * @brief Calculates an aabb bounding box for the current object.
     * @return AABB that encompasses the current object.
     */
    [[nodiscard]] virtual Aabb bounding_box() const = 0;
};

#endif