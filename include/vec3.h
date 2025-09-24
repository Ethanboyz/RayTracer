#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

template <class Tag> class Vec3;
template<class Tag> inline constexpr bool dependent_false_v = false;

struct any_tag {};      // Regular vec3
struct unit_tag {};     // Unit vector version of vec3

using coord3 = Vec3<any_tag>;
using vec3 = Vec3<any_tag>;
using Color = Vec3<any_tag>;
using uvec3 = Vec3<unit_tag>;

template <class Tag> class Vec3 {
public:
    // Constructors
    constexpr Vec3() : s{0.f, 0.f, 0.f} {}
    constexpr Vec3(const float x, const float y, const float z) : s{x, y, z} {
        // No need to normalize if already length ≈ 1
        if (std::is_same_v<Tag, unit_tag> && std::abs(length_squared() - 1) < 1.0e-7) {
            normalize();
        }
    }
    // Fail conversions between different Vec3 types, enforce use of unit() and nounit()
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr explicit Vec3(const vec3&) : s{0.f, 0.f, 0.f} {
        static_assert(dependent_false_v<T>, "Cannot initialize uvec3 from vec3. Use unit(v) to create a unit vector uvec3 instead.");
    }
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr explicit Vec3(const uvec3&) : s{0.f, 0.f, 0.f} {
        static_assert(dependent_false_v<T>, "Cannot initialize vec3 from uvec3. Use nounit(v) to create a vector vec3 instead.");
    }

    // Accessors
    constexpr float x() const noexcept { return s[0]; }
    constexpr float y() const noexcept { return s[1]; }
    constexpr float z() const noexcept { return s[2]; }

    // Operator overloading
    Vec3 operator-() const noexcept {   // Unary '-' operator
        return Vec3{
            -s[0],
            -s[1],
            -s[2]
        };
    }

    // Mutating operators can only work on regular vec3, should be disabled for uvec3
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator+=(const Vec3& other) noexcept {
        s[0] += other.s[0];
        s[1] += other.s[1];
        s[2] += other.s[2];
        return *this;
    }
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator-=(const Vec3& other) noexcept {
        s[0] -= other.s[0];
        s[1] -= other.s[1];
        s[2] -= other.s[2];
        return *this;
    }
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator*=(const float t) noexcept {
        s[0] *= t;
        s[1] *= t;
        s[2] *= t;
        return *this;
    }
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr Vec3& operator/=(const float t) {
        s[0] /= t;
        s[1] /= t;
        s[2] /= t;
        return *this;
    }
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator+=(const Vec3& other) noexcept = delete;
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator-=(const Vec3& other) noexcept = delete;
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator*=(const Vec3& other) noexcept = delete;
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr void operator/=(const Vec3& other) noexcept = delete;

    // In case of printing vectors
    constexpr friend std::ostream& operator<<(std::ostream& stream, const Vec3& v) noexcept {
        stream << "[" << v.s[0] << ", " << v.s[1] << ", " << v.s[2] << "]";
        return stream;
    }

    // Scalar-vector arithmetic
    constexpr friend vec3 operator*(const Vec3& v, const float t) noexcept {
        return vec3{v.s[0] * t, v.s[1] * t, v.s[2] * t};
    }
    constexpr friend vec3 operator*(const float t, const Vec3& v) noexcept {
        return v * t;
    }
    constexpr friend vec3 operator/(const Vec3& v, const float t) {
        return vec3{v.s[0] / t, v.s[1] / t, v.s[2] / t};
    }

    // Vector-vector arithmetic
    constexpr friend vec3 operator+(const Vec3& u, const Vec3& v) noexcept {
        return vec3{u.s[0] + v.s[0], u.s[1] + v.s[1], u.s[2] + v.s[2]};
    }
    constexpr friend vec3 operator-(const Vec3& u, const Vec3& v) noexcept {
        return vec3{u.s[0] - v.s[0], u.s[1] - v.s[1], u.s[2] - v.s[2]};
    }
    constexpr friend float dot(const Vec3& u, const Vec3& v) {
        return u.s[0] * v.s[0] + u.s[1] * v.s[1] + u.s[2] * v.s[2];
    }

    constexpr friend vec3 cross(const Vec3& u, const Vec3& v) noexcept {
        return vec3{
            u.s[1] * v.s[2] - u.s[2] * v.s[1],
            u.s[2] * v.s[0] - u.s[0] * v.s[2],
            u.s[0] * v.s[1] - u.s[1] * v.s[0],
        };
    }

    // Misc
    constexpr float length_squared() const noexcept {
        return s[0] * s[0] + s[1] * s[1] + s[2] * s[2];
    }
    constexpr float length() const noexcept {
        return std::sqrt(this->length_squared());
    }
    // Convert from vec3 to uvec3
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, any_tag>, int> = 0>
    constexpr friend uvec3 unit(const Vec3& v) noexcept {
        const Vec3 res = v / v.length();
        return uvec3{res.x(), res.y(), res.z()};
    }
    // Convert from uvec3 to vec3
    template<class T = Tag, std::enable_if_t<std::is_same_v<T, unit_tag>, int> = 0>
    constexpr friend vec3 nounit(const Vec3& v) noexcept {
        return vec3{v.x(), v.y(), v.z()};
    }

private:
    // Scalar components
    float s[3];

    void normalize() {
        const float len = this->length();
        if (len > 0) {
            s[0] /= len;
            s[1] /= len;
            s[2] /= len;
        }
    }
};

#endif