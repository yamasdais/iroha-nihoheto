#pragma once
#include <concepts>
#include <iterator>
#include <numeric>
#include <cmath>

namespace challenge100 {

template <class T>
concept gcd_type = requires(T v) {
    requires std::integral<T>;
    requires !std::same_as<T, bool>;
    {v % v} -> std::convertible_to<T>;
};

template <gcd_type T>
constexpr T gcd(T a, T b)
    noexcept
{
    return (b == 0) ? a : gcd(b, a % b);
}

template <gcd_type T>
constexpr T lcm(T a, T b)
    noexcept
{
    auto h = gcd(a, b);
    return h ? (a * (b / h)) : 0;
}

template <class Iter, class T = std::iter_value_t<Iter>>
    requires std::forward_iterator<Iter> && gcd_type<T>
constexpr auto lcmr(Iter s, Iter e)
    noexcept
{
    return std::accumulate(s, e, static_cast<T>(1), lcm<T>);
}

template <class Num>
    requires std::integral<Num>
bool is_prime(Num num) {
    if (num < 2) return false;
    else if (num == 2) return true;
    else if ((num & 0x1) == 0) return false;

    double end = std::sqrt(static_cast<double>(num));
    for (Num i = 3; i <= end; i += 2) {
        if (num % i == 0)
            return false;
    }
    return true;
}


}

