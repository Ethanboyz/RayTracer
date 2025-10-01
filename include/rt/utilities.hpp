#ifndef UTILITIES_H
#define UTILITIES_H

#include <random>
#include "math/interval.hpp"

namespace Utilities {
    /**
     * @brief Generates a random value inside the Interval.
     * @param range Interval the random value will be generated between.
     * @return Random value.
     */
    inline float random_float(const Interval<float> range) noexcept {
        std::random_device rd;
        static std::mt19937 engine{rd()};
        static std::uniform_real_distribution distribution(range.min(), range.max());
        return distribution(engine);
    }
}

#endif