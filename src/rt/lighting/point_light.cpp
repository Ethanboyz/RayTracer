#include "rt/lighting/point_light.hpp"

void PointLight::light_intensity(HitRecord& hit_record) const {
    float new_light_intensity{hit_record.light_intensity()};

    // Diffuse lighting
    const vec3 L{position_ - hit_record.point()};           // L = vec3 from hit point to point light
    const uvec3 N{hit_record.normal()};

    if (const float n_dot_l{dot(N, L)}; n_dot_l > 0) {
        float added_light_intensity{intensity_ * n_dot_l / L.length()};
        added_light_intensity *= hit_record.material().diffuse();
        new_light_intensity += added_light_intensity;
    }
    if (new_light_intensity > 1) {
        new_light_intensity = 1;
    }
    hit_record.light_intensity(new_light_intensity);
}