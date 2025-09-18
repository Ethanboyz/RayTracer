#ifdef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
public:
    float elements[3];

    // Very simple vectors
    vec3() : elements{0, 0, 0} {}
    vec3(float x, float y, float z) : elements{x, y, z} {}

};
#endif