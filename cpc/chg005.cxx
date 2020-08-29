#include <iostream>
#include <algorithm>
#include <ranges>
#include <vector>
#include "challenge.h"

template <std::integral Num>
bool check_sexy_primes(Num base, int count) {
    auto primes = std::vector<Num>{};
    primes.reserve(count);
    auto nums = std::ranges::iota_view{0, count}
                | std::views::transform([base](auto const n) { return base + n * 6; });
    // std::ranges::copy(nums, std::back_inserter(primes));
    if (std::all_of(std::ranges::cbegin(nums), std::ranges::cend(nums),
                    [&primes](auto const v) {
                        if (cpc::is_prime(v)) {
                            primes.push_back(v);
                            return true;
                        }
                        return false;
                     })) {
        std::cout << "sexy prime numbers: ";
        for (auto i : primes) {
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
        check_sexy_primes(i, 4);
    }

    return 0;
}
