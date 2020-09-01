#include <iostream>
#include <ranges>
#include <numeric>
#include <cmath>
#include <array>

template <int width>
using ValArray = std::array<int, width>;

auto makeIota(int index) {
    return std::ranges::iota_view(index == 0 ? 1 : 0, 10);
}

template <int width>
auto makeValue(int index, int value, ValArray<width>& ary) {
    ary[index] = value;
}

template <int width>
auto printValue(ValArray<width> const& ary) {
    for (auto v : ary) {
        std::cout << v;
    }
    std::cout << ",";
}

template <int width>
auto calcValue(ValArray<width> const& ary) {
    return std::accumulate(std::cbegin(ary), std::cend(ary), 0,
            [](auto acc, auto i) { acc *= 10; return acc + i; });
}

template <int width>
auto calcArmstrong(ValArray<width> const& ary) {
    return std::accumulate(std::cbegin(ary), std::cend(ary), 0,
            [](auto acc, auto i) { return acc + std::pow(i, width); });
}

template <int width>
void makeArray(ValArray<width>& ary, int index = 0) {
    auto r = std::ranges::iota_view(index, width);
    for (auto n : r) {
        auto iio = makeIota(n);
        for (auto i : iio) {
            makeValue<width>(n, i, ary);
            if (n == index && index < width - 1) {
                //std::cout << n << "@" << index << "|";
                makeArray<width>(ary, n + 1);
            } else if (index == width - 1 && n == width - 1) {
                //std::cout << "[" << n << "@" << index << "]";
                auto sum = calcValue<width>(ary);
                auto arm = calcArmstrong<width>(ary);
                if (sum == arm) {
                    //std::cout << "[" << sum << "," << arm << "]";
                    printValue<width>(ary);
                }
            }
        }
    }
}

int main(int, char**) {
    auto constexpr width = 4;
    auto ary = std::array<int, width>();
    std::cout << std::boolalpha;
    makeArray<width>(ary);
    std::cout << std::endl;
    return 0;
}

