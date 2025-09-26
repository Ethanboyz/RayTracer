#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "hittable.h"
#include "light.h"

class PointLight : public Light {
public:
    constexpr explicit PointLight(const float intensity, const coord3 position) : iy{intensity}, pos{position} {}

    // Updates hit_record light intensity by adding the light intensity caused by the current light to the current hit_record's light intensity
    constexpr void light_intensity(HitRecord& hit_record) const override {
        float new_light_intensity = hit_record.light_intensity();

        // Diffuse lighting
        const vec3 L = pos - hit_record.point();                                                // L = vec3 from hit point to point light
        const uvec3 N{hit_record.front_face() ? hit_record.normal() : -hit_record.normal()};    // Make sure N is the outward facing normal
        const float n_dot_l = dot(N, L);
        if (n_dot_l > 0) {
            float added_light_intensity{iy * n_dot_l/(N.length() * L.length())};
            added_light_intensity *= hit_record.material().diffuse();
            new_light_intensity += added_light_intensity;
        }
        if (new_light_intensity > 1) {
            new_light_intensity = 1;
        }
        hit_record.light_intensity(new_light_intensity);
    }

private:
    float iy;
    coord3 pos;
};

#endif