#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>
#include <type_traits>

template<class T>
class Interval {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>, "Intervals must be numerical.");
public:
    constexpr Interval() : lower{std::numeric_limits<T>::lowest()}, upper{std::numeric_limits<T>::max()} {}
    constexpr Interval(const T min, const T max) : lower{min}, upper{max} {}

    // Accessors
    constexpr T min() const noexcept { return lower; }
    constexpr T max() const noexcept { return upper; }

    // Returns the range of the interval (can be negative if min is greater than max)
    constexpr T range() const noexcept { return upper - lower; }

    // Returns true if x is within the interval range (inclusive)
    constexpr bool inclusive_contains(const T x) const noexcept { return lower <= x && x <= upper; }

    // Returns true if x is within the interval range (exclusive)
    constexpr bool exclusive_contains(const T x) const noexcept { return lower < x && x < upper; }

private:
    T lower, upper;
};

#endif