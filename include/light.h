#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "vec3.h"

class Light {
public:
    virtual ~Light() = default;
    virtual constexpr void light_intensity(HitRecord& hit_record) const = 0;
};

#endif