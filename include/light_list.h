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

    // Populate hit_record with the cumulative light intensity from world lights
    constexpr void light_intensity(HitRecord& hit_record) const override {
        for (const shared_ptr<Light>& light : lts) {
            // Diffuse materials lighting
            light->light_intensity(hit_record);
        }
    };

private:
    std::vector<shared_ptr<Light>> lts;
};

#endif
