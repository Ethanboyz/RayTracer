#ifndef AABB_H
#define AABB_H

#include <stdexcept>
#include "rt/math/interval.hpp"

class HitRecord;
class Ray;

/**
 * @class Aabb
 * @brief Basic axis-aligned bounding box implementation where world objects and other aabb's live.
 */
class Aabb {
public:
    /** @brief Constructs a new effectively empty axis-aligned bounding box.*/
    constexpr Aabb() :
        x_{Interval<float>{1, 0}},
        y_{Interval<float>{1, 0}},
        z_{Interval<float>{1, 0}} {}

    /** @brief Constructs a new axis-aligned bounding box that encloses two other aabb's. */
    constexpr Aabb(const Aabb& box_a, const Aabb& box_b) :
        x_{Interval{box_a.x(), box_b.x()}},
        y_{Interval{box_a.y(), box_b.y()}},
        z_{Interval{box_a.z(), box_b.z()}} {}

    /**
     * @brief Constructs a new axis-aligned bounding box within the specified x, y, z boundaries.
     * @param x X-axis boundary of the bounding box.
     * @param y Y-axis boundary of the bounding box.
     * @param z Z-axis boundary of the bounding box.
     */
    constexpr Aabb(const Interval<float>& x, const Interval<float>& y, const Interval<float>& z) : x_{x}, y_{y}, z_{z} {}


    // Accessors
    /** @return X-axis range that bounds the aabb. */
    [[nodiscard]] constexpr Interval<float> x() const noexcept { return x_; }
    /** @return Y-axis range that bounds the aabb. */
    [[nodiscard]] constexpr Interval<float> y() const noexcept { return y_; }
    /** @return Z-axis range that bounds the aabb. */
    [[nodiscard]] constexpr Interval<float> z() const noexcept { return z_; }

    /**
     * @brief Subscript operation for read-only access of elements.
     * @param i Index of aabb.
     * @return Interval of the axis from the corresponding i.
     */
    constexpr Interval<float>& operator[](const std::size_t i) {
        if (i >= 3) {
            throw std::out_of_range("Index out of bounds for Vec3");
        }
        if (i == 0) { return x_; }
        if (i == 1) { return y_; }
        return z_;
    }

    /**
     * @brief Subscript operation for read-only access of elements.
     * @param i Index of aabb.
     * @return Interval of the axis from the corresponding i.
     */
    constexpr const Interval<float>& operator[](const std::size_t i) const {
        if (i >= 3) {
            throw std::out_of_range("Index out of bounds for Vec3");
        }
        if (i == 0) { return x_; }
        if (i == 1) { return y_; }
        return z_;
    }

    /**
     *
     * @param ray Checked for intersections with the current aabb object.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @return True if ray intersects the current aabb, false otherwise.
     */
    bool ray_hit(const Ray& ray, Interval<float>& t) const;
private:
    Interval<float> x_, y_, z_;
};

#endif