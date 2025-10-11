#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt/math/vec3.hpp"
class Ray;

using Emittance = float;
using Reflectance = float;
using Shininess = float;
using Refraction = float;
using RefractionIndex = float;

/**
 * @class Material
 * @brief Stores material properties of a surface (i.e. base color and reflective properties).
 */
class Material {
public:
    // Named constructors
    /**
     * @brief Constructs a new opaque reflective Material with the specified properties.
     * @param albedo Acts as the base color of the Material.
     * @param reflectance Chance of rays that should be scattered/reflected in some way (0.0-1.0). Lower values = more rays getting absorbed.
     * @param shininess Chance of non-absorbed rays that should be reflected specularly (0.0-1.0). Lower values = more diffuse.
     */
    constexpr static Material create_reflective_material(const Color& albedo, const float reflectance, const float shininess) noexcept {
        return Material{albedo, Emittance{0}, reflectance, shininess, Refraction{0}, RefractionIndex{0}};
    }

    /**
     * @brief Constructs a new refractive Material with the specified properties.
     * @param albedo Acts as the base color of the Material.
     * @param refraction Chance of rays that should be refracted (0.0-1.0). Lower values = more opaque.
     * @param refraction_index Refraction index of the Material.
     */
    constexpr static Material create_refractive_material(const Color& albedo, const float refraction, const float refraction_index) noexcept {
        return Material{albedo, Emittance{0}, Reflectance{1 - refraction}, Shininess{0}, refraction, refraction_index};
    }

    /**
     * @brief Constructs a new light source Material.
     * @param color Color of the emitted light.
     * @param emittance Intensity of the emitted light.
     */
    constexpr static Material create_light(const Color& color, const float emittance) noexcept {
        return Material{color, emittance, Reflectance{0}, Shininess{0}, Refraction{0}, RefractionIndex{0}};
    }

    // Accessors
    /** @return Base color of the Material. */
    [[nodiscard]] constexpr Color albedo() const noexcept { return albedo_; }
    /** @return Emittance of the Material. */
    [[nodiscard]] constexpr float emittance() const noexcept { return emittance_; }
    /** @return Reflectance reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float reflectance() const noexcept { return reflectance_; }
    /** @return Shininess reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float shininess() const noexcept { return shininess_; }
    /** @return Refractiveness of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float refraction() const noexcept { return refraction_; }
    /** @return Refractive index of the Material. */
    [[nodiscard]] constexpr float refraction_index() const noexcept { return refraction_index_; }

    /** @param albedo Sets the albedo of the current Material. */
    constexpr void color(const Color& albedo) noexcept { albedo_ = albedo; }
    /** @param emittance Sets the emittance. */
    constexpr void emittance(const float emittance) noexcept { emittance_ = emittance; }
    /** @param reflectance Sets the reflectance. */
    constexpr void reflectance(const float reflectance) noexcept { reflectance_ = reflectance; }
    /** @param shininess Sets the shininess reflective component. */
    constexpr void shininess(const float shininess) noexcept { shininess_ = shininess; }
    /** @param refraction Sets the refractiveness. */
    constexpr void refraction(const float refraction) noexcept { refraction_ = refraction; }
    /** @param refraction_index Sets the refraction index. */
    constexpr void refraction_index(const float refraction_index) noexcept { refraction_index_ = refraction_index; }

private:
    Color albedo_;                          // Base color before any shading or effects
    float emittance_;                       // Light emission intensity
    float reflectance_, shininess_;         // Material reflective properties
    float refraction_, refraction_index_;   // Chance of refraction, refraction index

    /**
     * @brief Constructs a new Material with the specified properties.
     *
     * @note Reflectance and refraction should not sum up to be more than 1.
     * @param albedo Acts as the base color of the Material.
     * @param emittance Light emission intensity of the Material.
     * @param reflectance Chance of rays that should be scattered/reflected in some way (0.0-1.0). Lower values = more rays getting absorbed.
     * @param shininess Chance of non-absorbed rays that should be reflected specularly (0.0-1.0). Lower values = more diffuse.
     * @param refraction Chance of rays that should be refracted (0.0-1.0). Lower values = more opaque.
     * @param refraction_index Refraction index of the Material.
     */
    constexpr Material(const Color& albedo, const float emittance, const float reflectance, const float shininess,  const float refraction, const float refraction_index) :
        albedo_{albedo},
        emittance_{emittance},
        reflectance_{Interval{0.f, 1.f}.clamp(reflectance)},
        shininess_{Interval{0.f, 1.f}.clamp(shininess)},
        refraction_{Interval{0.f, 1.f}.clamp(refraction)},
        refraction_index_{refraction_index} {
        if (const float sum{reflectance + refraction}; sum > 1) {
            reflectance_ /= sum;
            refraction_ /= sum;
        }
    }
};

#endif