#ifndef VEC3_H
#define VEC3_H

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include "rt/utilities.hpp"

class HitRecord;

template <class Tag> class Vec3;
template <class Tag> inline constexpr bool dependent_false_v = false;

struct any_tag {};      // Regular vec3
struct unit_tag {};     // Unit vector version of vec3

using coord3 = Vec3<any_tag>;       // 3D coordinates (functionally the same as vec3)
using Color = Vec3<any_tag>;        // R, G, B values (0-1)
using vec3 = Vec3<any_tag>;         // 3D vectors
using uvec3 = Vec3<unit_tag>;       // 3D unit vectors (length ≈ 1)

/**
 * @class Vec3
 * @brief Represents a 3D vector.
 * @note Also used for coordinates and RGB colors.
 * @tparam Tag Determines the type of vector (unit vector with asserted length ≈ 1 or regular vector).
 */
template <class Tag> class Vec3 {
public:
    /** @brief Constructs a 3D vector with all components initialized to 0. */
    constexpr Vec3() : s{0.f, 0.f, 0.f} {}

    /**
     * @brief Constructs a 3D vector with the specified scalar values.
     * @param x First vector component.
     * @param y Second vector component.
     * @param z Third vector component.
     */
    constexpr Vec3(const float x, const float y, const float z) : s{x, y, z} {
        // No need to normalize if already length ≈ 1
        if (std::is_same_v<Tag, unit_tag> && std::fabs(length_squared() - 1) < 1.0e-6) {
            normalize();
        }
    }
    /** @brief Cannot perform conversions between Vec3 types */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr explicit Vec3(const vec3&) : s{0.f, 0.f, 0.f} {
        static_assert(dependent_false_v<T>, "Cannot initialize uvec3 from vec3. Use unit(v) to create a unit vector uvec3 instead.");
    }
    /** @brief Cannot perform conversions between Vec3 types */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr explicit Vec3(const uvec3&) : s{0.f, 0.f, 0.f} {
        static_assert(dependent_false_v<T>, "Cannot initialize vec3 from uvec3. Use nounit(v) to create a vector vec3 instead.");
    }

    // Accessors
    /** @return First vector component. */
    [[nodiscard]] constexpr float x() const noexcept { return s[0]; }
    /** @return Second vector component. */
    [[nodiscard]] constexpr float y() const noexcept { return s[1]; }
    /** @return Third vector component. */
    [[nodiscard]] constexpr float z() const noexcept { return s[2]; }

    // Operator overloading
    /**
     * @brief Unary negation operation.
     * @return Vec3 with all components negated.
     */
    constexpr Vec3 operator-() const noexcept {   // Unary '-' operator
        return Vec3{
            -s[0],
            -s[1],
            -s[2]
        };
    }

    /**
     * @brief Mutating vector-vector addition operation.
     * @note Only vec3 (non-unit vectors) can be the lvalue of mutating operators.
     * @param other Rvalue of the += operation.
     * @return Result of vector addition of lvalue on rvalue.
     */
    template<class T = Tag, class OtherTag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator+=(const Vec3<OtherTag>& other) noexcept {
        s[0] += other.x();
        s[1] += other.y();
        s[2] += other.z();
        return *this;
    }

    /**
     * @brief Mutating vector-vector subtraction operation.
     * @note Only vec3 (non-unit vectors) can be the lvalue of mutating operators.
     * @param other Rvalue of the -= operation.
     * @return Result of vector subtraction of rvalue subtracted from lvalue.
     */
    template<class T = Tag, class OtherTag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator-=(const Vec3<OtherTag>& other) noexcept {
        s[0] -= other.x();
        s[1] -= other.y();
        s[2] -= other.z();
        return *this;
    }

    /**
     * @brief Mutating vector-scalar multiplication operation.
     * @note Only vec3 (non-unit vectors) can use mutating operators.
     * @param t Rvalue of the *= operation (scalar to multiply the lvalue by).
     * @return Result of multiplication of lvalue and rvalue.
     */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator*=(const float t) noexcept {
        s[0] *= t;
        s[1] *= t;
        s[2] *= t;
        return *this;
    }

    /**
     * @brief Mutating vector-vector Hadamard product operation.
     * @note Only vec3 (non-unit vectors can use mutating operators).
     * @param other Rvalue of the *= operation.
     * @return Result of multiplying the ith vector components together for i = 1, 2, 3.
     */
    template<class T = Tag, class OtherTag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator*=(const Vec3<OtherTag>& other) noexcept {
        s[0] *= other.x();
        s[1] *= other.y();
        s[2] *= other.z();
        return *this;
    }

    /**
     * @brief Mutating vector-scalar division operation.
     * @note Only vec3 (non-unit vectors) can use mutating operators.
     * @param t Rvalue of the /= operation (scalar to divide the lvalue by).
     * @return Result of division of lvalue by rvalue.
     */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator/=(const float t) {
        s[0] /= t;
        s[1] /= t;
        s[2] /= t;
        return *this;
    }

    /**
     * @brief Subscript operation for modification of elements.
     * @param i Index of vector.
     * @return Vector component from the corresponding i.
     */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr float& operator[](const std::size_t i) {
        if (i >= 3) {
            throw std::out_of_range("Index out of bounds for Vec3");
        }
        return s[i];
    }

    /**
     * @brief Subscript operation for read-only access of elements.
     * @param i Index of vector.
     * @return Vector component from the corresponding i.
     */
    constexpr const float& operator[](const std::size_t i) const {
        if (i >= 3) {
            throw std::out_of_range("Index out of bounds for Vec3");
        }
        return s[i];
    }

    /** @brief Cannot perform mutating operations on uvec3 */
    template<class T = Tag, class OtherTag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator+=(const Vec3<OtherTag>& other) noexcept = delete;
    /** @brief Cannot perform mutating operations on uvec3 */
    template<class T = Tag, class OtherTag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator-=(const Vec3<OtherTag>& other) noexcept = delete;
    /** @brief Cannot perform mutating operations on uvec3 */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator*=(float t) noexcept = delete;
    /** @brief Cannot perform mutating operations on uvec3 */
    template<class T = Tag, class OtherTag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator*=(const Vec3<OtherTag>& other) noexcept = delete;
    /** @brief Cannot perform mutating operations on uvec3 */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator/=(float t) noexcept = delete;

    /**
     * @brief Vector stream insertion operation.
     * @note Formats vectors in [x, y, z] format.
     * @param stream Lvalue of the << oepration (stream to insert vector into).
     * @param v Rvalue of the << operation (vector to insert).
     * @return Same stream used as the Lvalue of the << operation.
     */
    constexpr friend std::ostream& operator<<(std::ostream& stream, const Vec3& v) noexcept {
        stream << "[" << v.s[0] << ", " << v.s[1] << ", " << v.s[2] << "]";
        return stream;
    }

    // Scalar-vector arithmetic
    /**
     * @brief Binary vector-scalar multiplication operation.
     * @param v Left side of the operation.
     * @param t Right side of the operation.
     * @return Vector-scalar product.
     */
    constexpr friend vec3 operator*(const Vec3& v, const float t) noexcept {
        return vec3{v.s[0] * t, v.s[1] * t, v.s[2] * t};
    }

    /**
     * @brief Binary scalar-vector multiplication operation.
     * @param v Left side of the operation.
     * @param t Right side of the operation.
     * @return Scalar-vector product.
     */
    constexpr friend vec3 operator*(const float t, const Vec3& v) noexcept {
        return v * t;
    }

    /**
     * @brief Binary vector-scalar division operation.
     * @param v Left side of the operation.
     * @param t Right side of the operation.
     * @return Vector-scalar quotient.
     */
    constexpr friend vec3 operator/(const Vec3& v, const float t) {
        return vec3{v.s[0] / t, v.s[1] / t, v.s[2] / t};
    }

    // Vector-vector arithmetic
    /**
     * @brief Binary vector-vector addition operation.
     * @param u Left side of the operation.
     * @param v Right side of the operation.
     * @return Vector-vector sum.
     */
    constexpr friend vec3 operator+(const Vec3& u, const Vec3& v) noexcept {
        return vec3{u.s[0] + v.s[0], u.s[1] + v.s[1], u.s[2] + v.s[2]};
    }

    /**
     * @brief Binary vector-vector subtraction operation.
     * @param u Left side of the operation.
     * @param v Right side of the operation.
     * @return Vector-vector difference.
     */
    constexpr friend vec3 operator-(const Vec3& u, const Vec3& v) noexcept {
        return vec3{u.s[0] - v.s[0], u.s[1] - v.s[1], u.s[2] - v.s[2]};
    }

    /**
     * @brief Binary vector-vector Hadamard product operation.
     * @param u Left side of the operation.
     * @param v Right side of the operation.
     * @return Vector-vector Hadamard product.
     */
    constexpr friend vec3 operator*(const Vec3& u, const Vec3& v) noexcept {
        return {u.s[0] * v.s[0], u.s[1] * v.s[1], u.s[2] * v.s[2]};
    }

    /**
     * @brief Vector-vector dot operation.
     * @param u Left side of the operation.
     * @param v Right side of the operation.
     * @return Vector-vector dot product.
     */
    template<class A, class B>
    constexpr friend float dot(const Vec3<A>& u, const Vec3<B>& v) noexcept;

    /**
     * @brief Vector-vector cross operation.
     * @param u Left side of the operation.
     * @param v Right side of the operation.
     * @return Vector-vector cross product.
     */
    constexpr friend vec3 cross(const Vec3& u, const Vec3& v) noexcept {
        return vec3{
            u.s[1] * v.s[2] - u.s[2] * v.s[1],
            u.s[2] * v.s[0] - u.s[0] * v.s[2],
            u.s[0] * v.s[1] - u.s[1] * v.s[0],
        };
    }

    // Misc
    /** @return True if the vector is degenerate (all components are or near zero). */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    [[nodiscard]] constexpr bool degenerate() const noexcept {
        constexpr float near_zero{1e-4};
        return std::fabs(s[0]) < near_zero && std::fabs(s[1]) < near_zero && std::fabs(s[2]) < near_zero;
    }

    /** @return Length of the vector, squared. */
    [[nodiscard]] constexpr float length_squared() const noexcept {
        return s[0] * s[0] + s[1] * s[1] + s[2] * s[2];
    }

    /**
     * @brief Calculates the length of the vector.
     * @note More expensive operation than length_squared().
     * @return Length of the vector.
     */
    [[nodiscard]] constexpr float length() const noexcept {
        return std::sqrt(this->length_squared());
    }

    /**
     * @brief Constructs a new uvec3 based off the provided vec3.
     * @note Should be used to convert vec3 to uvec3. The given vec3 is normalized and the result is returned.
     * @param v vec3 to convert into uvec3.
     * @return Normalized uvec3 created from the given vec3.
     */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr friend uvec3 unit(const Vec3& v) noexcept {
        const Vec3 res = v / v.length();
        return uvec3{res.x(), res.y(), res.z()};
    }

    /**
     * @brief Constructs a new vec3 based off the provided uvec3.
     * @note Should be used to convert uvec3 to vec3. Everything but the new vector's type remains unchanged.
     * @param v uvec3 to convert into vec3.
     * @return vec3 created from the given uvec3.
     */
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr friend vec3 nounit(const Vec3& v) noexcept {
        return vec3{v.x(), v.y(), v.z()};
    }

private:
    // Scalar components
    float s[3];

    /** @brief Normalizes vectors. */
    constexpr void normalize() {
        if (const float len{this->length()}; len > 0) {
            s[0] /= len;
            s[1] /= len;
            s[2] /= len;
        }
    }
};

template<class A, class B>
constexpr float dot(const Vec3<A>& u, const Vec3<B>& v) noexcept {
    return u.s[0] * v.s[0] + u.s[1] * v.s[1] + u.s[2] * v.s[2];
}

// Utilities
/**
 * @brief Generates a random direction unit vector to trace ray scattering.
 *
 * Randomization is cosine-weighted where directions closer to the normal
 * are prioritized.
 * @note Because scatter is only computed on incident rays from the outward-facing side, the incident and outward-facing
 * normals are the same.
 * @param normal Normal vector.
 * @return Randomly-generated unit vector that adheres to the above criteria.
 */
[[nodiscard]] uvec3 scatter_uvec3(const uvec3& normal);

/**
 * @brief Generates a direction unit vector of another vector, reflected (to trace ray reflection).
 * @param v Direction vector to be reflected.
 * @param normal Incident ray-facing normal vector (not to be confused with the outward-facing normal).
 * @return Reflected direction vector that adheres to the above criteria.
 */
[[nodiscard]] uvec3 reflect_uvec3(const uvec3& v, const uvec3& normal);

/**
 * @brief Generates a direction unit vector of another vector, refracted (to trace ray refraction) or reflected if total internal reflection applies.
 * @param v Direction vector to be refracted.
 * @param normal Incident ray-facing normal vector (not to be confused with the outward-facing normal).
 * @param eta Refractive index of the incident medium (medium before ray-object intersect).
 * @param eta_prime Refractive index of the new medium (medium after ray-object intersect).
 * @return Refracted direction vector that adheres to the above criteria (unless reflected due to total internal reflection).
 */
[[nodiscard]] uvec3 refract_uvec3(const uvec3& v, const uvec3& normal, float eta, float eta_prime);

#endif