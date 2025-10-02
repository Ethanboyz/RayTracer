#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt/math/vec3.hpp"
class Ray;

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
     * @param reflectance % of rays that should be scattered/reflected in some way. 1-reflectance = chance of ray getting absorbed.
     * @param shininess % of scattered rays that should be reflected specularly.
     * @param emittance Light emission intensity of the Material.
     */
    constexpr Material(const Color &albedo, const float emittance, const float reflectance, const float shininess)
        : albedo_{albedo}, emittance_{emittance}, reflectance_{reflectance}, shininess_{shininess} {
        if (const float sum{reflectance_ + shininess_}; sum > 1) {
            reflectance_ /= sum;
            shininess_ /= sum;
        }
    }

    // Accessors
    /** @return Base color of the Material. */
    [[nodiscard]] constexpr Color albedo() const noexcept { return albedo_; }
    /** @return Emittance of the Material. */
    [[nodiscard]] constexpr float emittance() const noexcept { return shininess_; }
    /** @return Reflectance reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float reflectance() const noexcept { return reflectance_; }
    /** @return Shininess reflective component of the Material (0.0-1.0). */
    [[nodiscard]] constexpr float shininess() const noexcept { return shininess_; }

    /** @param albedo Sets the albedo of the current Material. */
    constexpr void color(const Color& albedo) noexcept { albedo_ = albedo; }
    /** @param emittance Sets the emittance. */
    constexpr void emittance(const float emittance) noexcept { emittance_ = emittance; }
    /** @param reflectance Sets the reflectance reflective component. */
    constexpr void reflectance(const float reflectance) noexcept { reflectance_ = reflectance; }
    /** @param shininess Sets the shininess reflective component. */
    constexpr void shininess(const float shininess) noexcept { shininess_ = shininess; }

    /**
     * @brief Determines if the current ray hitting the surface should continue as scattered child rays, outputting
     * the next scattered ray and a color depending on the current Material.
     *
     * @param ray Parent ray hitting the surface.
     * @param hit_record HitRecord of the ray-object intersection.
     * @param attenuation Updated with the attenuation after the hit.
     * @param scattered Updated with the child ray after this hit.
     * @return True if a scattered child ray was generated as a result of this hit, false if the parent ray was absorbed.
     */
    bool scatter(const Ray& ray, const HitRecord& hit_record, Color& attenuation, Ray& scattered) const;

private:
    Color albedo_;                  // Base color before any shading or effects
    float emittance_;               // Light emission intensity
    float reflectance_, shininess_; // Material reflective properties
};

#endif