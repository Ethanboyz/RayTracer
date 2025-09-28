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
    constexpr Material(const Color &color, const float diffuse, const float specular) : color_{color}, diffuse_{diffuse}, specular_{specular} {
        if (const float sum{diffuse_ + specular_}; sum > 1) {
            diffuse_ /= sum;
            specular_ /= sum;
        }
    }

    // Accessors
    /** @return Base color of the Material. */
    [[nodiscard]] constexpr Color color() const noexcept { return color_; }
    /** @return Diffuse reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float diffuse() const noexcept { return diffuse_; }
    /** @return Specular reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float specular() const noexcept { return specular_; }

    /** @param color Sets the base color of the current Material. */
    constexpr void color(const Color& color) noexcept { color_ = color; }
    /** @param diffuse Sets the diffuse reflective component. */
    constexpr void diffuse(const float diffuse) noexcept { diffuse_ = diffuse; }
    /** @param specular Sets the specular reflective component. */
    constexpr void specular(const float specular) noexcept { specular_ = specular; }

private:
    Color color_;                   // Base color before any shading or effects
    float diffuse_, specular_;      // Material reflective properties
};

#endif