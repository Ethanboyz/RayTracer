#include <random>
#include "rt/utilities.hpp"

namespace Utilities {
    float random_float() {
        thread_local std::mt19937 engine{[] {
            std::random_device rd;
            return std::mt19937(rd());
        }()};
        std::uniform_real_distribution distribution(0.f, 1.f);
        return distribution(engine);
    }

    float random_float(const Interval<float>& range) {
        thread_local std::mt19937 engine{[] {
            std::random_device rd;
            return std::mt19937(rd());
        }()};
        std::uniform_real_distribution distribution(range.min(), range.max());
        return distribution(engine);
    }

    float degrees_to_radians(const float degrees) {
        return degrees * static_cast<float>(M_PI / 180.f);
    }
}
