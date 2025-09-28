#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

/**
 * @class Material
 * @brief Stores material properties of a surface (i.e. base color and reflective properties).
 */
class Material {
public:
    /**
     * @brief Constructs a new Material with the specified properties.
     *
     * @note diffuse and specular should not sum up to be more than 1.
     * @param color Base color of the Material before any kind of shading or effects.
     * @param diffuse % of light that should be scattered uniformly upon reflection.
     * @param specular % of light that should be reflected specularly.
     */
    constexpr Material(const Color &color, const float diffuse, const float specular) : col{color}, dif{diffuse}, spec{specular} {
        const float sum = dif + spec;
        if (sum > 1) {
            dif /= sum;
            spec /= sum;
        }
    }

    // Accessors
    /** @return Base color of the Material. */
    [[nodiscard]] constexpr Color color() const noexcept { return col; }
    /** @return Diffuse reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float diffuse() const noexcept { return dif; }
    /** @return Specular reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float specular() const noexcept { return spec; }

    /** @param color Sets the base color of the current Material. */
    constexpr void color(const Color& color) noexcept { col = color; }
    /** @param diffuse Sets the diffuse reflective component. */
    constexpr void diffuse(const float diffuse) noexcept { dif = diffuse; }
    /** @param specular Sets the specular reflective component. */
    constexpr void specular(const float specular) noexcept { spec = specular; }

private:
    Color col;              // Base color before any shading or effects
    float dif, spec;        // Material reflective properties
};

#endif