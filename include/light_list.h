#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <vector>
#include <memory>
#include "light.h"

using std::make_shared;
using std::shared_ptr;

// Replace list with some other acceleration structure
class LightList : public Light {
public:
    LightList() = default;
    explicit LightList(shared_ptr<Light> light) { lts.push_back(light); }

    void clear() { lts.clear(); }
    void add(shared_ptr<Light> light) { lts.push_back(light); }

    // Populate hit_record with the cumulative light intensity
    constexpr void light_intensity(HitRecord& hit_record) const override {
        float intensity = 0.f;
        for (const shared_ptr<Light>& light : lts) {
            // Diffuse materials lighting
            if (light.type() == AMBIENT) {
                intensity += light.intensity;
            } else {
                if (light.type == POINT) {
                    L = light.position - point_of_hit;
                }
                if (light.type == DIRECTIONAL) {
                    L = light.direction;
                }
                n_dot_l = dot(N, L);
                if n_dot_l > 0 {
                    i += light.intensity * n_dot_l/(length(N) * length(L));
                }
            }
        }
        hit_record.light_intensity(intensity);
    };

private:
    std::vector<shared_ptr<Light>> lts;
};

#endif
