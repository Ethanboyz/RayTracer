#ifndef RAY_H
#define RAY_H

#include "rt/math/vec3.hpp"

/**
 * @class Ray
 * @brief Implementation of a geometric ray type represented by a origin point and a direction.
 */
class Ray {
public:
    /**
     * @brief Constructs a Ray positioned at the origin with no direction.
     * @warning Rays with no direction are inherently useless.
     */
    Ray() = default;

    /**
     * @brief Constructs a Ray positioned at the specified coordinates extending out the direction.
     * @param position Coordinates of the starting point.
     * @param direction Direction the Ray extends towards.
     */
    Ray(const coord3& position, const uvec3& direction) : position_{position}, direction_{direction} {}

    [[nodiscard]] const constexpr coord3& origin() const noexcept { return position_; }
    [[nodiscard]] const constexpr uvec3& direction() const noexcept { return direction_; }

    [[nodiscard]] coord3 position(const float t) const noexcept { return coord3{position_ + t * direction_}; }

private:
    const coord3 position_;
    const uvec3 direction_;
};

#endif