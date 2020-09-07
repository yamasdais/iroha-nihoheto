#pragma once
#include <cmath>
#include <functional>
#include <concepts>
#include <iterator>
#include <array>
#include <vector>
#include <numeric>
#include <type_traits>

namespace challenge100 {

template <class>
struct ArraySizeDetector : public std::false_type {};

template <class T, size_t N>
struct ArraySizeDetector<std::array<T, N>> : public std::true_type {
    constexpr static size_t size = N;
};

struct accum_fn {
    template <std::forward_iterator IStart, std::sentinel_for<IStart> IEnd,
             class BinaryFn, class Init, class Proj = std::identity>
        requires std::convertible_to<std::invoke_result_t<BinaryFn, Init, std::iter_value_t<IStart>>, Init>
    constexpr Init operator()(IStart st, IEnd en, Init&& init, BinaryFn&& func, Proj proj = {}) const {
        Init ret = std::forward<Init>(init);
        for (; st != en; ++st) {
            ret = std::invoke(std::forward<BinaryFn>(func), ret, std::invoke(proj, *st));
        }
        return ret;
    }

    template <std::ranges::forward_range Range, class BinaryFn, class Init, class Proj = std::identity>
    constexpr Init operator()(Range&& range, Init&& init, BinaryFn&& func, Proj proj = {}) const {
        return (*this)(std::ranges::begin(range), std::ranges::end(range), std::forward<Init>(init),
               std::forward<BinaryFn>(func), std::move(proj));
    }
};

inline constexpr accum_fn accum = accum_fn{};

template <class T>
  requires std::ranges::range<T>
auto constexpr toIters(T& container)
-> std::pair<decltype(std::ranges::begin(container)), decltype(std::ranges::end(container))> {
    return std::make_pair(std::ranges::begin(container), std::ranges::end(container));
}

template <class T>
  requires std::ranges::range<T>
auto constexpr toIters(T const& container)
-> std::pair<decltype(std::ranges::cbegin(container)), decltype(std::ranges::cend(container))> {
    return std::make_pair(std::ranges::cbegin(container), std::ranges::cend(container));
}

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

template <std::unsigned_integral Num>
std::vector<Num> prime_factors(Num n) {
    std::vector<Num> ret;
    while (n % 2 == 0) {
        ret.push_back(2);
        n /= 2;
    }
    Num const rt = static_cast<Num>(std::sqrt(n));
    for (Num i = 3; i <= rt; i += 2) {
        while (n % i == 0) {
            ret.push_back(i);
            n /= i;
        }
    }

    if (n > 2)
        ret.push_back(n);

    return ret;
}



}  // namespace challenge100

namespace cpc = challenge100;
