#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:
    virtual ~Light() = default;

    // Adds to hit_record light_intensity with a brighter light_intensity depending on the current object's light properties
    virtual constexpr void light_intensity(HitRecord& hit_record) const = 0;
};

#endif