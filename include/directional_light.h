#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "hittable.h"
#include "vec3.h"
#include "light.h"

class DirectionalLight final : public Light {
public:
    //constexpr DirectionalLight() : iy{0.f} {}
    //constexpr explicit DirectionalLight(const float intensity, const Ray direction) : iy{intensity}, pos_dir{direction}  {}

    // Updates hit_record light intensity by adding the light intensity caused by the current light to the current hit_record's light intensity
    constexpr void light_intensity(HitRecord& hit_record) const override {
        float new_light_intensity = hit_record.light_intensity();

        // Diffuse lighting
        //Ray L = pos_dir;
        //n_dot_l = dot(N, L);
        //if n_dot_l > 0 {
        //    new_light_intensity += iy * n_dot_l/(length(N) * length(L));
        //    new_light_intensity *= hit_record.material().diffuse();
        //}
        if (new_light_intensity > 1) {
            new_light_intensity = 1;
        }
    }

private:
    //float iy;
    Ray pos_dir;
};

#endif