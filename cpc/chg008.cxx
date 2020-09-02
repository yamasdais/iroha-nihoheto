#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>

template <size_t width>
using ValArray = std::array<unsigned long, width>;

auto makeIota(size_t index) {
    return std::ranges::iota_view(index == 0 ? 1u : 0u, 10u);
}

template <class Iter, class T>
    requires std::random_access_iterator<Iter>&&
        std::assignable_from<std::iter_reference_t<Iter>, T>
auto makeValue(size_t index, T value, Iter ptr) {
    ptr[index] = value;
}

template <class Iter>
auto printValue(Iter const b, Iter const e) {
    std::for_each(b, e, [](auto const n) { std::cout << n; });
    std::cout << ",";
}

template <class T>
auto calcValue(T const& ary) {
    return std::accumulate(std::cbegin(ary), std::cend(ary), 0,
                           [](auto acc, auto i) {
                               acc *= 10;
                               return acc + i;
                           });
}

template <class Iter>
requires std::sized_sentinel_for<Iter, Iter>
auto calcArmstrong(Iter const b, Iter const e) {
    auto size = std::distance(b, e);
    return std::accumulate(b, e, 0,
        [size](auto acc, auto i) { return acc + std::pow(i, size); });
}

template <size_t width>
void makeArray(ValArray<width>& ary, size_t index = 0) {
    auto r = std::ranges::iota_view(index, width);
    for (auto n : r) {
        auto iio = makeIota(n);
        for (auto i : iio) {
            // makeValue<width>(n, i, ary);
            makeValue(n, i, std::ranges::begin(ary));
            if (n == index && index < width - 1) {
                // std::cout << n << "@" << index << "|";
                makeArray<width>(ary, n + 1);
            } else if (index == width - 1 && n == width - 1) {
                // std::cout << "[" << n << "@" << index << "]";
                auto sum = calcValue(ary);
                auto arm = calcArmstrong(std::ranges::cbegin(ary), std::ranges::cend(ary));
                if (sum == arm) {
                    // std::cout << "[" << sum << "," << arm << "]";
                    printValue(std::ranges::cbegin(ary), std::ranges::cend(ary));
                }
            }
        }
    }
}

int main(int, char**) {
    auto constexpr width = 4;
    auto ary = ValArray<width>();
    std::cout << std::boolalpha;
    makeArray<ary.size()>(ary);
    std::cout << std::endl;
    return 0;
}

