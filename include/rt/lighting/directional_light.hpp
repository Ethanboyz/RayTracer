#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "rt/geom/hittable.hpp"
#include "rt/math/vec3.hpp"
#include "rt/lighting/light.hpp"

/**
 * @class DirectionalLight
 * @brief Light source that emits in a single direction from an infinitely far source.
 */
class DirectionalLight final : public Light {
public:
    /**
     * @brief Constructs a DirectionalLight in world space with the given properties at the specified location.
     * @param intensity Intensity of the DirectionalLight (higher intensity means brighter).
     * @param direction Direction the DirectionalLight emits from.
     */
    constexpr explicit DirectionalLight(const float intensity, const uvec3 direction) : intensity_{intensity}, direction_{direction} {}

    /**
     * @brief Adds the light intensity caused by the current light to the current hit_record's light intensity.
     * @param hit_record Updated with the new light intensity after the current DirectionalLight is applied to it.
     */
    void light_intensity(HitRecord& hit_record) const override;

private:
    float intensity_;           // Light intensity
    uvec3 direction_;           // Direction from where the light emits
};

#endif