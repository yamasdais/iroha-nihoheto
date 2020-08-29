#pragma once
#include <cmath>
#include <concepts>
#include <iterator>
#include <numeric>

namespace challenge100 {

template <class T>
concept gcd_type = requires(T v) {
    requires std::integral<T>;
    requires !std::same_as<T, bool>;
    { v % v }
    ->std::convertible_to<T>;
};

template <gcd_type T>
constexpr T gcd(T a, T b) noexcept {
    return (b == 0) ? a : gcd(b, a % b);
}

template <gcd_type T>
constexpr T lcm(T a, T b) noexcept {
    auto h = gcd(a, b);
    return h ? (a * (b / h)) : 0;
}

template <class Iter, class T = std::iter_value_t<Iter>>
requires std::forward_iterator<Iter>&& gcd_type<T> constexpr auto lcmr(
  Iter s, Iter e) noexcept {
    return std::accumulate(s, e, static_cast<T>(1), lcm<T>);
}

template <std::integral Num>
bool is_prime(Num num) {
    if (num <= 3)
        return num > 1;
    else if ((num & 0x1) == 0 || num % 3 == 0)
        return false;

    Num rt = std::sqrt(num);
    for (Num i = 5; i <= rt; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

template <std::integral Num>
Num sum_proper_divisors(Num const number) {
    Num result = 1;
    Num const rt = static_cast<Num>(std::sqrt(number));
    for (Num i = 2; i <= rt; i++) {
        if (number % i == 0) {
            result += (i == (number / i)) ? i : (i + number / i);
        }
    }

    return result;
}


}  // namespace challenge100

namespace cpc = challenge100;
