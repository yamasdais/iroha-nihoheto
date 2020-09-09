#include <iostream>
#include <concepts>
#include <bit>
#include <bitset>
#include <climits>
#include <ranges>

#include "challenge.h"
#include "sequence_view.h"

// グレイコード

template <std::unsigned_integral T>
T gray_encode(T const num) {
    return num ^ (num >> 1);
}

template <std::unsigned_integral T>
T gray_decode(T gray) {
    T ret = static_cast<T>(0u);
    ret |= gray;
    while (gray) {
        gray >>= 1;
        ret ^= gray;
    }
    //for (auto bit = std::rotr(static_cast<T>(2u), 1); bit > 1; bit >>= 1) {
    //    if (gray & bit) gray ^= bit >> 1;
    //}
    return ret;
}

template <std::unsigned_integral T>
std::string to_binary(T const value, size_t const digits) {
    constexpr size_t bitwidth = sizeof(T) * CHAR_BIT;
    return std::bitset<bitwidth>(value).to_string().substr(bitwidth-digits, digits);
}

int main(int, char**) {
    std::cout << "Number\tBinary\tGray\tDecoded" << std::endl;
    std::cout << "------\t------\t----\t-------" << std::endl;
    for (auto n : std::ranges::iota_view(0u, 32u)) {
        auto encoded = gray_encode(n);
        auto decoded = gray_decode(encoded);

        std::cout << n << "\t" << to_binary(n, 5) << "\t"
            << to_binary(encoded, 5) << "\t" << decoded << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
