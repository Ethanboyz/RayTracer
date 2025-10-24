#ifndef INTERVAL_H
#define INTERVAL_H

#include <type_traits>

/**
 * @class Interval
 * @brief Represents an interval/range between a lower and upper bound.
 * @tparam T Interval values type (must be arithmetic).
 */

using std::fmin;
using std::fabs;

template<class T>
class Interval {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>, "Intervals must be numerical.");
public:
    /** @brief Constructs an effectively empty Interval. */
    constexpr Interval() : min_{1e-6}, max_{-1e-6} {}

    /**
     * @brief Constructs an Interval from min to max.
     * @param min Interval lower bound.
     * @param max Interval upper bound.
     */
    constexpr Interval(const T min, const T max) : min_{min}, max_{max} {}

    /** @brief Constructs an Interval that encloses two other Intervals. */
    constexpr Interval(const Interval& a, const Interval& b)
      : min_{a.min() <= b.min() ? a.min() : b.min()},
        max_{a.max() >= b.max() ? a.max() : b.max()} {}

    // Accessors
    /** @return Lower bound of current Interval. */
    [[nodiscard]] constexpr T min() const noexcept { return min_; }
    /** @return Upper bound of current Interval. */
    [[nodiscard]] constexpr T max() const noexcept { return max_; }
    /** @param min Sets lower bound of current Interval. */
    constexpr void min(const T& min) noexcept { min_ = min; }
    /** @param max Sets upper bound of current Interval. */
    constexpr void max(const T& max) noexcept { max_ = max; }

    /**
     * @brief Calculates the range of the current Interval.
     * @warning Can return negative values if min is greater than max.
     * @return upper bound - lower bound.
     */
    [[nodiscard]] constexpr T range() const noexcept { return max_ - min_; }

    /** @return True if the Interval's min bound is strictly larger than the max bound */
    [[nodiscard]] constexpr bool is_empty() const noexcept {return min_ > max_; }

    /** @return True if x is within the interval range (inclusive). */
    [[nodiscard]] constexpr bool inclusive_contains(const T x) const noexcept { return min_ <= x && x <= max_; }

    /** @return True if x is within the interval range (inclusive) with epsilon tolerance. */
    [[nodiscard]] constexpr bool inclusive_contains(const T x, float epsilon) const noexcept {
        epsilon = fmin(fabs(epsilon), range());
        return min_ <= x + fabs(epsilon) && x <= max_ - fabs(epsilon);
    }

    /** @return True if x is within the interval range (exclusive). */
    [[nodiscard]] constexpr bool exclusive_contains(const T x) const noexcept { return min_ < x && x < max_; }

    /** @return x if within the interval, or the nearest Interval boundary otherwise. */
    [[nodiscard]] constexpr T clamp(const T x) const noexcept {
        if (x < min_) return min_;
        if (x > max_) return max_;
        return x;
    }

private:
    T min_, max_;
};

#endif