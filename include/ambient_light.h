#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "hittable.h"
#include "light.h"

/**
 * @class AmbientLight
 * @brief Acts as a global light source.
 */
class AmbientLight final : public Light {
public:
    /** @brief Constructs a new AmbientLight with light intensity 0. */
    constexpr AmbientLight() : intensity_{0.f} {}

    /** @brief Constructs a new AmbientLight with the specified light intensity.
     * @param intensity Intensity of the AmbientLight (increases light level globally).
     */
    constexpr explicit AmbientLight(const float intensity) : intensity_{intensity} {}

    /**
     * @brief Calculates a new light intensity for the passed HitRecord.
     *
     * Adds the light intensity caused by the current light to the current hit_record's light intensity.
     * @param hit_record Updated with the new light intensity after the current AmbientLight is applied to it.
     */
    void light_intensity(HitRecord& hit_record) const noexcept override {
        // Ambient light behavior is the same regardless of whether the material is diffuse/specular
        float new_light_intensity{hit_record.light_intensity() + intensity_};

        // Diffuse + specular should sum to ~1
        new_light_intensity *= hit_record.material().diffuse() + hit_record.material().specular();
        if (new_light_intensity > 1) {
            new_light_intensity = 1;
        }
        hit_record.light_intensity(new_light_intensity);
    }

private:
    float intensity_;
};

#endif