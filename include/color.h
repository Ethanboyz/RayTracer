#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

using Color = Vec3;

// Output a specific color to a pixel
inline void write_color(std::ostream& outstr, const Color& pixel_color) {
    const float r = pixel_color.x();
    const float g = pixel_color.y();
    const float b = pixel_color.z();

    const int ir = static_cast<int>(255.999 * r);
    const int ig = static_cast<int>(255.999 * g);
    const int ib = static_cast<int>(255.999 * b);

    // Output color components PPM-style
    outstr << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif
