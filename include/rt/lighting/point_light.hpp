#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "rt/geom/hittable.hpp"
#include "rt/lighting/light.hpp"

/**
 * @class PointLight
 * @brief Light source that emits in all directions from a single location. Light gets weaker over distance.
 */
class PointLight final : public Light {
public:
    /**
     * @brief Constructs a PointLight in world space with the given properties at the specified location.
     * @param intensity Intensity of the PointLight (higher intensity means brighter over farther distances).
     * @param position Coordinate position of the PointLight.
     */
    constexpr PointLight(const float intensity, const coord3 position) : intensity_{intensity}, position_{position} {}

    /**
     * @brief Adds the light intensity caused by the current light to the current hit_record's light intensity.
     * @param hit_record Updated with the new light intensity after the current PointLight is applied to it.
     */
    void light_intensity(HitRecord& hit_record) const override;

private:
    float intensity_;       // Light intensity
    coord3 position_;       // Coordinate position
};

#endif