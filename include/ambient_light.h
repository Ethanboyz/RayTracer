#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "light.h"

class AmbientLight : public Light {
public:
    constexpr AmbientLight() : iy{0.f} {}
    constexpr explicit AmbientLight(const float intensity) : iy{intensity} {}

private:
    float iy;
};

#endif