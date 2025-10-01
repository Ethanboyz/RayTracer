#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt/math/vec3.hpp"

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
     * @param albedo Acts as the base color of the Material.
     * @param diffuse % of light that should be scattered uniformly upon reflection.
     * @param specular % of light that should be reflected specularly.
     * @param emittance Light emission intensity of the Material.
     */
    constexpr Material(const Color &albedo, const float emittance, const float diffuse, const float specular)
        : albedo_{albedo}, emittance_{emittance}, diffuse_{diffuse}, specular_{specular} {
        if (const float sum{diffuse_ + specular_}; sum > 1) {
            diffuse_ /= sum;
            specular_ /= sum;
        }
    }

    // Accessors
    /** @return Base color of the Material. */
    [[nodiscard]] constexpr Color albedo() const noexcept { return albedo_; }
    /** @return Emittance of the Material. */
    [[nodiscard]] constexpr float emittance() const noexcept { return specular_; }
    /** @return Diffuse reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float diffuse() const noexcept { return diffuse_; }
    /** @return Specular reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float specular() const noexcept { return specular_; }

    /** @param albedo Sets the albedo of the current Material. */
    constexpr void color(const Color& albedo) noexcept { albedo_ = albedo; }
    /** @param emittance Sets the emittance. */
    constexpr void emittance(const float emittance) noexcept { emittance_ = emittance; }
    /** @param diffuse Sets the diffuse reflective component. */
    constexpr void diffuse(const float diffuse) noexcept { diffuse_ = diffuse; }
    /** @param specular Sets the specular reflective component. */
    constexpr void specular(const float specular) noexcept { specular_ = specular; }

private:
    Color albedo_;                  // Base color before any shading or effects
    float emittance_;               // Light emission intensity
    float diffuse_, specular_;      // Material reflective properties
};

#endif