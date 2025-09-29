#include "rt/lighting/directional_light.hpp"

void DirectionalLight::light_intensity(HitRecord &hit_record) const {
    float light_intensity{hit_record.light_intensity()};
    const uvec3 L{direction_};
    const uvec3 N{hit_record.normal()};

    if (const float n_dot_l{dot(N, L)}; n_dot_l > 0) {
        // Diffuse lighting
        float added_light_intensity{intensity_ * n_dot_l};
        added_light_intensity *= hit_record.material().diffuse();
        if (light_intensity > 1) {
            light_intensity = 1;
        }
        light_intensity += added_light_intensity;
        static constexpr Interval interval{0.f, 1.f};
        light_intensity = interval.clamp(light_intensity);
    }
    hit_record.light_intensity(light_intensity);
}