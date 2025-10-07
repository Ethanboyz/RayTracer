#ifndef AABB_H
#define AABB_H

#include <cmath>
#include <stdexcept>
#include "rt/math/interval.hpp"
#include "rt/math/vec3.hpp"

class HitRecord;
class Ray;

/**
 * @class Aabb
 * @brief Basic axis-aligned bounding box implementation where world objects and other aabb's live.
 */
class Aabb {
public:
    /** @brief Constructs a new effectively empty axis-aligned bounding box.*/
    constexpr Aabb() = default;

    /** @brief Constructs a new axis-aligned bounding box that encloses two other aabb's. */
    constexpr Aabb(const Aabb& box_a, const Aabb& box_b) :
        x_{Interval{box_a.x(), box_b.x()}},
        y_{Interval{box_a.y(), box_b.y()}},
        z_{Interval{box_a.z(), box_b.z()}} {}

    /** @brief Constructs a new axis-aligned bounding box that encloses two points. */
    constexpr Aabb(const coord3& a, const coord3& b) :
        x_{a[0] <= b[0] ? Interval{a[0], b[0]} : Interval{b[0], a[0]}},
        y_{a[1] <= b[1] ? Interval{a[1], b[1]} : Interval{b[1], a[1]}},
        z_{a[2] <= b[2] ? Interval{a[2], b[2]} : Interval{b[2], a[2]}} {}

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

    // Operator overloads
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

    /** @return Center coordinate of the Aabb. */
    [[nodiscard]] constexpr coord3 centroid() const noexcept {
        const coord3 sum{x_.min() + x_.max(), y_.min() + y_.max(), z_.min() + z_.max()};
        return sum * 0.5f;
    }

    /** @return Surface area of the Aabb. */
    [[nodiscard]] constexpr float surface_area() const noexcept {
        if (x_.is_empty() || y_.is_empty() || z_.is_empty()) {
            return 0;
        }
        const float x_len{x_.range()};
        const float y_len{y_.range()};
        const float z_len{z_.range()};
        return 2.f * x_len * y_len + y_len * z_len + x_len * z_len;
    }

    /**
     * @brief Checks for Ray intersections with the current Aabb.
     * @param ray Checked for intersections with the current Aabb object.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @return True if ray intersects the current aabb, false otherwise.
     */
    [[nodiscard]] bool ray_hit(const Ray& ray, Interval<float> t) const;

    /** @return True if the Aabb is degenerate (no volume) or "empty." */
    [[nodiscard]] constexpr bool is_degenerate() const {
        if (x_.is_empty() || y_.is_empty() || z_.is_empty()) { return true; }

        constexpr float near_zero{1e-4};
        return x_.range() < near_zero || y_.range() < near_zero || z_.range() < near_zero;
    }
private:
    Interval<float> x_, y_, z_;
};

#endif