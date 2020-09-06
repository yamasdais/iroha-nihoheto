#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
#include <type_traits>

#include "challenge.h"

template <size_t width>
using ValArray = std::array<unsigned long, width>;

template <class T, size_t I>
void buildAry(T& ary, std::index_sequence<I>) {
    auto constexpr size = cpc::ArraySizeDetector<std::decay_t<decltype(ary)>>::size;
    auto iter = std::ranges::iota_view(I == 0 ? 1u : 0u, 10u);
    for (auto i : iter) {
        ary[I] = i;
        auto [start, end] = cpc::toIters(ary);
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

template <class T, size_t I, size_t... Itail>
void buildAry(T& ary, std::index_sequence<I, Itail...>) {
    auto iter = std::ranges::iota_view(I == 0 ? 1u : 0u, 10u);
    for (auto i : iter) {
        ary[I] = i;
        buildAry(ary, std::index_sequence<Itail...>{});
    }
}

template <class T>
requires std::ranges::random_access_range<T>
void buildAryVar(T& ary, size_t index = 0) {
    auto iter = std::ranges::iota_view(index == 0 ? 1u : 0u, 10u);
    auto size = ary.size();
    auto remain = size - index;
    for (auto i : iter) {
        ary[index] = i;
        if (remain > 1) {
            buildAryV(ary, index + 1);
        } else {
            auto [start, end] = cpc::toIters(ary);
            auto sum = std::accumulate(start, end, 0,
                    [](auto acc, auto n) { return acc * 10 + n; });
            auto arm = std::accumulate(start, end, 0,
                    [size](auto acc, auto i) { return acc + std::pow(i, size); });
            if (sum == arm) {
                // std::cout << "[" << sum << "," << arm << "]";
                std::for_each(start, end, [](auto const n) { std::cout << n; });
                std::cout << ",";
            }
        }
    }
} 

template <class T>
    requires std::ranges::random_access_range<T>
        && std::ranges::sized_range<T>
void constexpr makeNums(T& ary) {
    if constexpr (cpc::ArraySizeDetector<std::decay_t<decltype(ary)>>::value) {
        auto constexpr sz = cpc::ArraySizeDetector<std::decay_t<decltype(ary)>>::size;
        buildAry(ary, std::make_index_sequence<sz>{});
    } else {
        buildAryVar(ary);
    }
}

int main(int, char**) {
    auto ary = std::array<unsigned long, 3>();
    makeNums(ary);
    std::cout << std::endl;
    return 0;
}

