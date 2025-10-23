#ifndef UTILITIES_H
#define UTILITIES_H

#include "math/interval.hpp"
#include <cstdint>

namespace Utilities {
    /** @brief Seed the random value generator for when the random_float() functions are called. */
    void seed_random_generator(uint64_t seed);

    /** @return Random value between 0 and 1 from a continuous uniform distribution. */
    float random_float();

    /** @return Random value between the bounds of the specified Interval range from a continuous uniform distribution. */
    float random_float(const Interval<float>& range);

    /** @brief Convert degrees to radians. */
    float degrees_to_radians(float degrees);
}

#endif