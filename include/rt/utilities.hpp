#ifndef UTILITIES_H
#define UTILITIES_H

#include "math/interval.hpp"

namespace Utilities {
    /** @return Random value between 0 and 1 from a continuous uniform distribution. */
    float random_float();

    /** @return Random value between the bounds of the specified Interval range from a continuous uniform distribution. */
    float random_float(const Interval<float>& range);

    /** @return Random value between the bounds of the specified Interval range. */
    int random_int(const Interval<int>& range);

    /** @brief Convert degrees to radians. */
    float degrees_to_radians(float degrees);
}

#endif