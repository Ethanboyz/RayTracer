#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class Vec3;
using Coord3 = Vec3;

class Vec3 {
public:
    constexpr Vec3() : s{0.f, 0.f, 0.f} {}
    constexpr Vec3(const float x, const float y, const float z) : s{x, y, z} {}

    // Accessors
    constexpr float x() const noexcept { return s[0]; }
    constexpr float y() const noexcept { return s[1]; }
    constexpr float z() const noexcept { return s[2]; }

    // Operator overloading
    Vec3& operator+=(const Vec3& other) noexcept {
        s[0] += other.s[0];
        s[1] += other.s[1];
        s[2] += other.s[2];
        return *this;
    }
    Vec3 operator-() const noexcept {   // Unary '-' operator
        return Vec3{
            -s[0],
            -s[1],
            -s[2]
        };
    }
    Vec3& operator-=(const Vec3& other) noexcept {
        s[0] -= other.s[0];
        s[1] -= other.s[1];
        s[2] -= other.s[2];
        return *this;
    }
    Vec3& operator*=(const float t) noexcept {
        s[0] *= t;
        s[1] *= t;
        s[2] *= t;
        return *this;
    }
    Vec3& operator/=(const float t) {
        s[0] /= t;
        s[1] /= t;
        s[2] /= t;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec3& v) noexcept {
        stream << "[" << v.s[0] << ", " << v.s[1] << ", " << v.s[2] << "]";
        return stream;
    }

    // Scalar-vector arithmetic
    friend Vec3 operator+(const Vec3& u, const Vec3& v) noexcept {
        return Vec3{u.s[0] + v.s[0], u.s[1] + v.s[1], u.s[2] + v.s[2]};
    }
    friend Vec3 operator-(const Vec3& u, const Vec3& v) noexcept {
        return Vec3{u.s[0] - v.s[0], u.s[1] - v.s[1], u.s[2] - v.s[2]};
    }
    friend Vec3 operator*(const Vec3& v, const float t) noexcept {
        return Vec3{v.s[0] * t, v.s[1] * t, v.s[2] * t};
    }
    friend Vec3 operator*(const float t, const Vec3& v) noexcept {
        return v * t;
    }
    friend Vec3 operator/(const Vec3& v, const float t) {
        return Vec3{v.s[0] / t, v.s[1] / t, v.s[2] / t};
    }

    // Vector-vector arithmetic
    friend float dot(const Vec3& u, const Vec3& v) {
        return u.s[0] * v.s[0] + u.s[1] * v.s[1] + u.s[2] * v.s[2];
    }

    friend Vec3 cross(const Vec3& u, const Vec3& v) {
        return Vec3{
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
    friend Vec3 unit(const Vec3& v) {
        return v / v.length();
    }

private:
    // Scalar components
    float s[3];
};

#endif