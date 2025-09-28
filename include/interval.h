#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>
#include <type_traits>

/**
 * @class Interval
 * @brief Represents an interval/range between a lower and upper bound.
 * @tparam T Interval values type (must be arithmetic).
 */
template<class T>
class Interval {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>, "Intervals must be numerical.");
public:
    /** @brief Constructs an Interval from the lowest possible to highest possible value, effectively an infinite interval. */
    constexpr Interval() : min_{std::numeric_limits<T>::lowest()}, max_{std::numeric_limits<T>::max()} {}

    /**
     * @brief Constructs an Interval from min to max.
     * @param min Interval lower bound.
     * @param max Interval upper bound.
     */
    constexpr Interval(const T min, const T max) : min_{min}, max_{max} {}

    // Accessors
    /** @return Lower bound of current Interval. */
    constexpr T min() const noexcept { return min_; }
    /** @return Upper bound of current Interval. */
    constexpr T max() const noexcept { return max_; }

    /**
     * @brief Calculates the range of the current Interval.
     * @warning Can return negative values if min is greater than max.
     * @return upper bound - lower bound.
     */
    constexpr T range() const noexcept { return max_ - min_; }

    // Returns true if x is within the interval range (inclusive)
    constexpr bool inclusive_contains(const T x) const noexcept { return min_ <= x && x <= max_; }

    // Returns true if x is within the interval range (exclusive)
    constexpr bool exclusive_contains(const T x) const noexcept { return min_ < x && x < max_; }

private:
    T min_, max_;
};

#endif