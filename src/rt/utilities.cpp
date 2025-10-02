#include <random>
#include "rt/utilities.hpp"

namespace Utilities {
    float random_float() noexcept {
        std::random_device rd;
        static std::mt19937 engine{rd()};
        static std::uniform_real_distribution distribution(0.f, 1.f);
        return distribution(engine);
    }

    float random_float(const Interval<float> range) noexcept {
        std::random_device rd;
        static std::mt19937 engine{rd()};
        static std::uniform_real_distribution distribution(range.min(), range.max());
        return distribution(engine);
    }
}
