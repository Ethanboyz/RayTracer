#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "vec3.h"

class Sphere {
public:
    Sphere(const Coord3& position, const float radius) : pos{position}, rad{radius} {}

    // Accessors
    constexpr Coord3 position() const noexcept { return pos; }
    constexpr float radius() const noexcept { return rad; }

    // Returns earliest t where ray intersects with the sphere
    float ray_hit(const Ray& r) const {
        const float a = {r.direction().length_squared()};
        const float b = {dot(r.direction(), pos - r.origin())};
        const float c = {(pos - r.origin()).length_squared() - (rad * rad)};
        const float det = {(b * b) - a * c};
        if (det < 0) {
            return -1.f;
        }
        return (b - std::sqrt(det)) / a; // Return only smallest possible t
    }

private:
    Coord3 pos;
    float rad;
};

#endif