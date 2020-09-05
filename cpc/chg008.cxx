#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
#include <type_traits>

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

template <class>
struct ArraySizeDetector : public std::false_type {};

template <typename T, size_t N>
struct ArraySizeDetector<std::array<T, N>> : public std::true_type {
    constexpr static size_t size = N;
};

template <class C, size_t I>
void buildAry(C& ary, std::index_sequence<I>) {
    auto constexpr size = ArraySizeDetector<std::decay_t<decltype(ary)>>::size;
    auto iter = std::ranges::iota_view(I == 0 ? 1u : 0u, 10u);
    for (auto i : iter) {
        ary[I] = i;
        auto start = std::ranges::begin(ary);
        auto end = std::ranges::end(ary);
        auto sum = std::accumulate(start, end, 0,
                [](auto acc, auto n) { return acc * 10 + n; });
        auto arm = std::accumulate(start, end, 0,
                [size](auto acc, auto i) { return acc + std::pow(i, size); });
        if (sum == arm) {
            std::for_each(start, end, [](auto const n) { std::cout << n; });
            std::cout << ",";
        }
    }
}

template <class C, size_t I, size_t... Itail>
void buildAry(C& ary, std::index_sequence<I, Itail...>) {
    auto iter = std::ranges::iota_view(I == 0 ? 1u : 0u, 10u);
    for (auto i : iter) {
        ary[I] = i;
        buildAry(ary, std::index_sequence<Itail...>{});
    }
}

template <class C>
    requires std::ranges::random_access_range<C>
        && std::ranges::sized_range<C>
void constexpr makeNums(C& ary) {
    if constexpr (ArraySizeDetector<std::decay_t<decltype(ary)>>::value) {
        auto constexpr sz = ArraySizeDetector<std::decay_t<decltype(ary)>>::size;
        buildAry(ary, std::make_index_sequence<sz>{});
    } else {
        auto sz = ary.size();
        accumNums(std::ranges::begin(ary), std::ranges::end(ary), 0);
    }
}

int main(int, char**) {
    auto constexpr width = 3;
    auto ary = ValArray<width>();
    makeNums(ary);
    std::cout << std::endl;
    return 0;
}

