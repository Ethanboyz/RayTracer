#include <random>
#include "rt/utilities.hpp"

namespace Utilities {
    static thread_local std::mt19937_64 engine{};
    void seed_random_generator(const uint64_t seed) {
        engine.seed(seed);
    }

    float random_float() {
        thread_local std::uniform_real_distribution distribution(0.f, 1.f);
        return distribution(engine);
    }

    float random_float(const Interval<float>& range) {
        return range.min() + (range.max() - range.min()) * random_float();
    }

    float degrees_to_radians(const float degrees) {
        return degrees * static_cast<float>(M_PI / 180.f);
    }
}
