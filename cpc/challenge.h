#pragma once
#include <concepts>
#include <numeric>

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

template <class Iter>
constexpr auto lcmr(Iter s, Iter e)
    requires gcd_type<typename Iter::value_type>
{
    using T = Iter::value_type;
    return std::accumulate(s, e, static_cast<T>(1), lcm<T>);
}


}

