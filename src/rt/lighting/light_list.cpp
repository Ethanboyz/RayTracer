#include "rt/lighting/light_list.hpp"

void LightList::light_intensity(HitRecord& hit_record) const {
    for (const shared_ptr<Light>& light : lights) {
        // Diffuse materials lighting
        light->light_intensity(hit_record);
    }
}