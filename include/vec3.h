//#ifdef VEC3_H
//#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
public:
    float s[3];

    // Very simple vectors
    constexpr vec3() : s{0.f, 0.f, 0.f} {}
    constexpr vec3(const float x, const float y, const float z) : s{x, y, z} {}

    // Accessors
    constexpr float x() const { return s[0]; }
    constexpr float y() const { return s[1]; }
    constexpr float z() const { return s[2]; }

    // Operator overloading
    vec3 operator+(const vec3& other) const noexcept {
        return vec3{
            s[0] + other.s[0],
            s[1] + other.s[1],
            s[2] + other.s[2]
        };
    }
    vec3& operator+=(const vec3& other) noexcept {
        s[0] += other.s[0];
        s[1] += other.s[1];
        s[2] += other.s[2];
        return *this;
    }
    vec3 operator-() const noexcept {   // Unary '-' operator
        return vec3{
            -s[0],
            -s[1],
            -s[2]
        };
    }
    vec3 operator-(const vec3& other) const noexcept {
        return vec3{
            s[0] - other.s[0],
            s[1] - other.s[1],
            s[2] - other.s[2]
        };
    }
    vec3& operator-=(const vec3& other) noexcept {
        s[0] -= other.s[0];
        s[1] -= other.s[1];
        s[2] -= other.s[2];
        return *this;
    }
    vec3 operator*(const float t) const noexcept {
        return vec3{
            s[0] * t,
            s[1] * t,
            s[2] * t
        };
    }
    vec3& operator*=(const float t) noexcept {
        s[0] *= t;
        s[1] *= t;
        s[2] *= t;
        return *this;
    }
    vec3 operator/(const float t) const {
        return vec3{
            s[0] / t,
            s[1] / t,
            s[2] / t
        };
    }
    vec3& operator/=(const float t) {
        s[0] /= t;
        s[1] /= t;
        s[2] /= t;
        return *this;
    }

};
//#endif