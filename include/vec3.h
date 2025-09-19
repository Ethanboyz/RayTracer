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
    vec3& operator-=(const vec3& other) noexcept {
        s[0] -= other.s[0];
        s[1] -= other.s[1];
        s[2] -= other.s[2];
        return *this;
    }
    vec3& operator*=(const float t) noexcept {
        s[0] *= t;
        s[1] *= t;
        s[2] *= t;
        return *this;
    }
    vec3& operator/=(const float t) {
        s[0] /= t;
        s[1] /= t;
        s[2] /= t;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec3& v) noexcept {
        stream << "[" << v.s[0] << ", " << v.s[1] << ", " << v.s[2] << "]";
        return stream;
    }

    // Scalar-vector arithmetic
    friend constexpr vec3 operator+(const vec3& u, const vec3& v) noexcept {
        return vec3{u.s[0] + v.s[0], u.s[1] + v.s[1], u.s[2] + v.s[2]};
    }
    friend vec3 operator-(const vec3& u, const vec3& v) noexcept {
        return vec3{u.s[0] - v.s[0], u.s[1] - v.s[1], u.s[2] - v.s[2]};
    }
    friend vec3 operator*(const vec3& v, const float t) noexcept {
        return vec3{v.s[0] * t, v.s[1] * t, v.s[2] * t};
    }
    friend vec3 operator*(const float t, const vec3& v) noexcept {
        return v * t;
    }
    friend vec3 operator/(const vec3& v, const float t) {
        return vec3{v.s[0] / t, v.s[1] / t, v.s[2] / t};
    }

    // Vector-vector arithmetic
    friend float dot(const vec3& u, const vec3& v) {
        return u.s[0] * v.s[0] + u.s[1] * v.s[1] + u.s[2] * v.s[2];
    }

    friend vec3 cross(const vec3& u, const vec3& v) {
        return vec3{
            u.s[1] * v.s[2] - u.s[2] * v.s[1],
            u.s[2] * v.s[0] - u.s[0] * v.s[2],
            u.s[0] * v.s[1] - u.s[1] * v.s[0],
        };
    }

    // Misc

    float length_squared() const noexcept {
        return s[0] * s[0] + s[1] * s[1] + s[2] * s[2];
    }
    float length() const noexcept {
        return std::sqrt(this->length_squared());
    }
    friend vec3 unit_vec3(const vec3& v) {
        return v / v.length();
    }
};

//#endif