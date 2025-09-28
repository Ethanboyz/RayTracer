#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "hittable.h"
#include "light.h"

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
    constexpr void light_intensity(HitRecord& hit_record) const override {
        float new_light_intensity = hit_record.light_intensity();

        // Diffuse lighting
        const vec3 L = position_ - hit_record.point();                                          // L = vec3 from hit point to point light
        const uvec3 N{hit_record.front_face() ? hit_record.normal() : -hit_record.normal()};    // Make sure N is the outward facing normal

        if (const float n_dot_l = dot(N, L); n_dot_l > 0) {
            float added_light_intensity{intensity_ * n_dot_l / L.length()};
            added_light_intensity *= hit_record.material().diffuse();
            new_light_intensity += added_light_intensity;
        }
        if (new_light_intensity > 1) {
            new_light_intensity = 1;
        }
        hit_record.light_intensity(new_light_intensity);
    }

private:
    float intensity_;       // Light intensity
    coord3 position_;       // Coordinate position
};

#endif