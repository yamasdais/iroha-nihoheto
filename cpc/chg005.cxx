#include <iostream>
#include <algorithm>
#include <ranges>
#include <array>
#include "challenge.h"

template <int Count, std::integral Num>
bool check_sexy_primes(Num base) {
    static_assert(Count > 1, "Count must be larger than 1");
    auto primes = std::array<Num, Count>();
    auto itrPrime = std::ranges::begin(primes);
    auto nums = std::ranges::iota_view{0, Count}
                | std::views::transform([base](auto const n) { return base + n * 6; });
    if (std::all_of(std::ranges::cbegin(nums), std::ranges::cend(nums),
                    [&itrPrime](auto const v) {
                        if (cpc::is_prime(v)) {
                            *itrPrime++ = v;
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
        check_sexy_primes<4>(i);
    }

    return 0;
}
