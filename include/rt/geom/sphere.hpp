#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "rt/math/ray.hpp"

/**
 * @class Sphere
 * @brief Sphere object.
 */
class Sphere final : public Hittable {
public:
    /**
     * @brief Construct a sphere in world space with the specified properties.
     * @param center Coordinate position of the sphere's center.
     * @param radius Radius of the sphere.
     * @param material Material of the entire sphere.
     */
    Sphere(const coord3& center, const float radius, const Material& material) : center_{center}, radius_{radius}, material_{material} {}

    // Accessors
    /** @return Coordinate position of the Sphere's center. */
    [[nodiscard]] constexpr coord3 position() const noexcept { return center_; }
    /** @return Radius of the current Sphere. */
    [[nodiscard]] constexpr float radius() const noexcept { return radius_; }
    /** @return Material of the current Sphere. Material applies to the entire Sphere. */
    [[nodiscard]] constexpr Material material() const noexcept { return material_; }

    /**
     * @brief Populates hit_record with Ray-Hittable intersect info if ray intersects the current Sphere.
     * @param ray Checked for intersections with the current Sphere object.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @param hit_record Updated with hit information of smallest t if ray intersection occurs.
     * @return True if ray intersects the current Sphere, false otherwise.
     */
    bool ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const override;

private:
    coord3 center_;         // Coordinate position of sphere's center
    float radius_;          // Sphere's radius
    Material material_;     // Material of sphere
};

#endif