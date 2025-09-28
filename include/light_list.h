#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <vector>
#include <memory>
#include "light.h"

using std::make_shared;
using std::shared_ptr;

/**
 * @class LightList
 * @brief Stores a list of Light sources.
 *
 * This can be passed into the renderer to render all contained objects.
 */
class LightList final : public Light {
public:
    /** @brief Constructs an empty LightList. */
    LightList() = default;

    void clear() { lights.clear(); }
    void add(shared_ptr<Light> light) { lights.push_back(light); }

    /**
     * @brief Calculates a new light intensity for the passed HitRecord.
     *
     * Adds the light intensity caused by all the Lights in the current LightList to the HitRecord's light intensity.
     * @param hit_record Updated with the new light intensity after the current LightList is applied to it.
     */
    void light_intensity(HitRecord& hit_record) const override {
        for (const shared_ptr<Light>& light : lights) {
            // Diffuse materials lighting
            light->light_intensity(hit_record);
        }
    };

private:
    std::vector<shared_ptr<Light>> lights;
};

#endif
