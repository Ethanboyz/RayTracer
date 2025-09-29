#include "rt/lighting/point_light.hpp"

void PointLight::light_intensity(HitRecord& hit_record) const {
    float light_intensity{hit_record.light_intensity()};

    // Diffuse lighting
    const vec3 L{position_ - hit_record.point()};           // L = vec3 from hit point to point light
    const uvec3 N{hit_record.normal()};

    if (const float n_dot_l{dot(N, L)}; n_dot_l > 0) {
        float added_light_intensity{intensity_ * n_dot_l / L.length()};
        added_light_intensity *= hit_record.material().diffuse();
        light_intensity += added_light_intensity;
    }
    static constexpr Interval interval{0.f, 1.f};
    light_intensity = interval.clamp(light_intensity);
    hit_record.light_intensity(light_intensity);
}