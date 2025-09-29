#include "rt/lighting/ambient_light.hpp"

void AmbientLight::light_intensity(HitRecord& hit_record) const noexcept {
    // Ambient light behavior is the same regardless of whether the material is diffuse/specular
    float new_light_intensity{hit_record.light_intensity() + intensity_};

    // Diffuse + specular should sum to ~1
    new_light_intensity *= hit_record.material().diffuse() + hit_record.material().specular();
    static constexpr Interval interval{0.f, 1.f};
    new_light_intensity = interval.clamp(new_light_intensity);
    hit_record.light_intensity(new_light_intensity);
}