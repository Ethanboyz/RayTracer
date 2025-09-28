#ifndef LIGHT_H
#define LIGHT_H

#include "rt/geom/hittable.hpp"

class Light {
public:
    virtual ~Light() = default;

    /**
     * @brief Calculates a new light intensity for the passed HitRecord.
     *
     * Adds the light intensity caused by the current light to the HitRecord's light intensity.
     * @param hit_record updated with the new light intensity after the current Light is applied to it.
     */
    virtual void light_intensity(HitRecord& hit_record) const = 0;
};

#endif