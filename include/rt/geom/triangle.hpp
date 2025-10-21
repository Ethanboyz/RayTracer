#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "rt/geom/hittable.hpp"
#include "rt/geom/aabb.hpp"

using std::fmin;
using std::fmax;

/**
 * @class Triangle
 * @brief Triangle object.
 */
class Triangle final : public Hittable {
public:
    /** @brief Constructs a new double-sided Triangle with the specified vertices a, b, c. */
    constexpr Triangle(const coord3& a, const coord3& b, const coord3& c, const Material& material) :
        a_{a},
        b_{b},
        c_{c},
        material_{material},
        bbox_{
            Interval{fmin(fmin(a.x(), b.x()), c.x()), fmax(fmax(a.x(), b.x()), c.x())},
            Interval{fmin(fmin(a.y(), b.y()), c.y()), fmax(fmax(a.y(), b.y()), c.y())},
            Interval{fmin(fmin(a.z(), b.z()), c.z()), fmax(fmax(a.z(), b.z()), c.z())}
        } {}

    // Accessors
    /** @return Material of the current Triangle. */
    [[nodiscard]] constexpr Material material() const noexcept { return material_; }

    /**
     * @brief Populates hit_record with Ray-Hittable intersect info if ray intersects the current object.
     * @param ray Checked for intersections with the current Hittable object.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @param hit_record Updated with hit information of smallest t if ray intersection occurs.
     * @return True if ray intersects the current object, false otherwise.
     */
    bool ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const override;

    /**
     * @brief Calculates an aabb bounding box for the current object.
     * @return AABB that encompasses the current object.
     */
    [[nodiscard]] Aabb bounding_box() const override { return bbox_; }

private:
    coord3 a_, b_, c_;              // Triangle vertices
    Material material_;
    Aabb bbox_;
};



#endif