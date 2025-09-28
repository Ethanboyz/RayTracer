#include "rt/lighting/directional_light.hpp"

void DirectionalLight::light_intensity(HitRecord &hit_record) const {
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