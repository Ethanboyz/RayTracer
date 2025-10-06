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
    constexpr Interval() :
        min_{std::numeric_limits<T>::lowest()},
        max_{std::numeric_limits<T>::max()} {}

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
    constexpr T min() const noexcept { return min_; }
    /** @return Upper bound of current Interval. */
    constexpr T max() const noexcept { return max_; }
    /** @param min Sets lower bound of current Interval. */
    constexpr void min(const T& min) noexcept { min_ = min; }
    /** @param max Sets upper bound of current Interval. */
    constexpr void max(const T& max) noexcept { max_ = max; }

    /**
     * @brief Calculates the range of the current Interval.
     * @warning Can return negative values if min is greater than max.
     * @return upper bound - lower bound.
     */
    constexpr T range() const noexcept { return max_ - min_; }

    /** @return True if x is within the interval range (inclusive). */
    constexpr bool inclusive_contains(const T x) const noexcept { return min_ <= x && x <= max_; }

    /** @return True if x is within the interval range (exclusive). */
    constexpr bool exclusive_contains(const T x) const noexcept { return min_ < x && x < max_; }

    /** @return x if within the interval, or the nearest Interval boundary otherwise. */
    constexpr T clamp(const T x) const noexcept {
        if (x < min_) return min_;
        if (x > max_) return max_;
        return x;
    }

private:
    T min_, max_;
};

#endif