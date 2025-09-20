#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

using color = vec3;

// Output a specific color to a pixel
inline void write_color(std::ostream& outstr, const color& pixel_color) {
    float r = pixel_color.x();
    float g = pixel_color.y();
    float b = pixel_color.z();

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    // Output color components PPM-style
    std::cout << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif
