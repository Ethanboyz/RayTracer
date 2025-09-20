#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "vec3.h"

class Sphere {
public:
    Sphere(const Coord3& position, const float radius) : pos{position}, rad{radius} {}

    // Determines if a ray intersects with the sphere
    bool ray_hit(const Ray& r) const {
        const float a = dot(r.direction(), r.direction());
        const float b = dot(-2 * r.direction(), pos - r.origin());
        const float c = dot(pos - r.origin(), pos - r.origin()) - (rad * rad);
        return (b * b) - 4 * a * c >= 0;    // If determinant is not 0, the ray intersects the sphere
    }

private:
    Coord3 pos;
    float rad;
};

#endif