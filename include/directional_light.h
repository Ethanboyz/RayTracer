#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "hittable.h"
#include "vec3.h"
#include "light.h"

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
    constexpr explicit DirectionalLight(const float intensity, const uvec3 direction) : intensity_{intensity}, direction_{direction}  {}

    /**
     * @brief Adds the light intensity caused by the current light to the current hit_record's light intensity.
     * @param hit_record Updated with the new light intensity after the current DirectionalLight is applied to it.
     */
    constexpr void light_intensity(HitRecord& hit_record) const override {
        float new_light_intensity{hit_record.light_intensity()};

        // Diffuse lighting
        const uvec3 L{direction_};
        const uvec3 N{hit_record.normal()};
        if (const float n_dot_l{dot(N, L)}; n_dot_l > 0) {
            float added_light_intensity{intensity_ * n_dot_l};
            added_light_intensity *= hit_record.material().diffuse();
            new_light_intensity += added_light_intensity;
        }
        if (new_light_intensity > 1) {
            new_light_intensity = 1;
        }
        hit_record.light_intensity(new_light_intensity);
    }

private:
    float intensity_;           // Light intensity
    uvec3 direction_;           // Direction from where the light emits
};

#endif