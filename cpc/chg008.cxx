#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>

template <size_t width>
using ValArray = std::array<unsigned long, width>;

auto makeIota(bool isFirst) {
    return std::ranges::iota_view(isFirst ? 1u : 0u, 10u);
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

template <class Iter>
requires std::sized_sentinel_for<Iter, Iter>
auto calcArmstrong(Iter const b, Iter const e) {
    auto size = std::distance(b, e);
    return std::accumulate(b, e, 0,
        [size](auto acc, auto i) { return acc + std::pow(i, size); });
}

template <class Iter>
void accumNums(Iter s, Iter e, size_t index) {
    auto iter = makeIota(index == 0);
    auto remain = std::distance(s, e) - index;
    for (auto i : iter) {
        makeValue(index, i, s);
        if (remain > 1) {
            accumNums(s, e, index + 1);
        } else {
            auto sum = std::accumulate(s, e, 0, [](auto acc, auto n) { return acc * 10 + n; });
            auto arm = calcArmstrong(s, e);
            if (sum == arm) {
                // std::cout << "[" << sum << "," << arm << "]";
                printValue(s, e);
            }
        }
    }
}

int main(int, char**) {
    auto constexpr width = 4;
    auto ary = ValArray<width>();
    //std::cout << std::boolalpha;
    accumNums(std::ranges::begin(ary), std::ranges::end(ary), 0);
    std::cout << std::endl;
    return 0;
}

