#include <iostream>
#include <algorithm>
#include <ranges>
#include "challenge.h"

template <std::integral Num>
bool check_sexy_primes(Num base, int count) {
    auto nums = std::ranges::iota_view{0, count}
                | std::ranges::views::transform([base](const auto n) { return base + n * 6; });
    if (std::all_of(std::ranges::cbegin(nums), std::ranges::cend(nums),
            [](auto v) { return challenge100::is_prime(v); })) {
        std::cout << "sexy prime numbers: ";
        for (auto i : nums) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    return true;
}

int main(int, char**) {
    unsigned long upper;
    std::cout << "Upper limit: ";
    std::cin >> upper;

    for (decltype(upper) i = 2; i <= upper; i++) {
        check_sexy_primes(i, 3);
    }

    return 0;
}
