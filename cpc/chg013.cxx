#include <iostream>
#include <array>
#include <algorithm>
#include <ranges>
#include <random>
#include <cmath>

#include "challenge.h"

template <class E, class D>
double compute_pi(E& engine, D& distrib, int const samples = 5'000'000) {
    auto r = std::ranges::views::iota(0, samples);
    auto hit = cpc::accum(r, 0, [&](auto acc, auto ) {
            auto x = distrib(engine);
            auto y = distrib(engine);
            if (y < std::sqrt(1 - std::pow(x, 2)))
                acc++;
            return acc;
            });
    return 4.0 * hit / samples;
}

int main(int, char**) {
    std::random_device rdev;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rdev));
    std::seed_seq seq(std::cbegin(seed_data), std::cend(seed_data));
    auto engine = std::mt19937{seq};
    auto distrib = std::uniform_real_distribution<>{0, 1};

    // std::ranges::for_each は <ranges> じゃなくて <algorithm> ヘッダに入ってるので注意
    std::ranges::for_each(std::ranges::views::iota(0, 10), [&engine, &distrib](auto ) {
            std::cout << compute_pi(engine, distrib) << std::endl;
            });
    return 0;
}
