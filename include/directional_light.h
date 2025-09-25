#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "light.h"

class DirectionalLight : public Light {
public:
    constexpr DirectionalLight() : iy{0.f} {}
    constexpr explicit DirectionalLight(const float intensity) : iy{intensity} {}

private:
    float iy;
};

#endif