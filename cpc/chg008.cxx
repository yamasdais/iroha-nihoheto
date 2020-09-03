#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>

template <size_t width>
using ValArray = std::array<unsigned long, width>;

template <class Iter>
    requires std::random_access_iterator<Iter>
    && std::assignable_from<std::iter_reference_t<Iter>, std::iter_value_t<Iter>>
    && std::sized_sentinel_for<Iter, Iter>
void accumNums(Iter s, Iter e, size_t index) {
    auto iter = std::ranges::iota_view(index == 0 ? 1u : 0u, 10u);
    auto size = std::distance(s, e);
    auto remain = size - index;
    for (auto i : iter) {
        s[index] = i;
        if (remain > 1) {
            accumNums(s, e, index + 1);
        } else {
            auto sum = std::accumulate(s, e, 0,
                    [](auto acc, auto n) { return acc * 10 + n; });
            auto arm = std::accumulate(s, e, 0,
                    [size](auto acc, auto i) { return acc + std::pow(i, size); });
            if (sum == arm) {
                // std::cout << "[" << sum << "," << arm << "]";
                std::for_each(s, e, [](auto const n) { std::cout << n; });
                std::cout << ",";
            }
        }
    }
}

int main(int, char**) {
    auto constexpr width = 4;
    auto ary = ValArray<width>();
    accumNums(std::ranges::begin(ary), std::ranges::end(ary), 0);
    std::cout << std::endl;
    return 0;
}

