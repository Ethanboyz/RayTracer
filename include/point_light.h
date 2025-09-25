#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.h"

class PointLight : public Light {
public:
    constexpr PointLight() : iy{0.f} {}
    constexpr explicit PointLight(const float intensity) : iy{intensity} {}

private:
    float iy;
};

#endif