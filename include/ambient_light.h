#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "hittable.h"
#include "light.h"

class AmbientLight : public Light {
public:
    constexpr AmbientLight() : iy{0.f} {}
    constexpr explicit AmbientLight(const float intensity) : iy{intensity} {}

    // Updates hit_record light intensity by adding the light intensity caused by the current light to the current hit_record's light intensity
    constexpr void light_intensity(HitRecord& hit_record) const override {
        // Ambient light behavior is the same regardless of whether the material is diffuse/specular
        float new_light_intensity = hit_record.light_intensity() + iy;

        // Diffuse + specular should sum to ~1
        new_light_intensity *= hit_record.material().diffuse() + hit_record.material().specular();
        if (new_light_intensity > 1) {
            new_light_intensity = 1;
        }
        hit_record.light_intensity(new_light_intensity);
    }

private:
    float iy;
};

#endif