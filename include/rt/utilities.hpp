#ifndef UTILITIES_H
#define UTILITIES_H

#include "math/interval.hpp"

namespace Utilities {
    /** @return Random value between 0 and 1. */
    float random_float();

    /** @return Random value between the bounds of the specified Interval range. */
    float random_float(Interval<float> range);

    /** @brief Convert degrees to radians. */
    float degrees_to_radians(float degrees);
}

#endif