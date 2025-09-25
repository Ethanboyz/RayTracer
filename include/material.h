#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

// Stores material properties of a surface, including its color and reflective properties
class Material {
public:
    constexpr Material(const Color &color, const float diffuse, const float specular) : col{color}, dif{diffuse}, spec{specular} {}

    // Accessors
    constexpr Color color() const noexcept { return col; }
    constexpr float diffuse() const noexcept { return dif; }
    constexpr float specular() const noexcept { return spec; }

    constexpr void color(const Color& color) noexcept { col = color; }
    constexpr void diffuse(const float diffuse) noexcept { dif = diffuse; }
    constexpr void specular(const float specular) noexcept { spec = specular; }

private:
    Color col;              // Base color before any shading or effects
    float dif, spec;        // Material reflective properties
};

#endif