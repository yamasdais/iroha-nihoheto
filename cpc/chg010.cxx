#include <bit>
#include <bitset>
#include <climits>
#include <cmath>
#include <concepts>
#include <iostream>
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
    // for (auto bit = std::rotr(static_cast<T>(1u), 1); bit > 1; bit >>= 1) {
    //    if (gray & bit) gray ^= bit >> 1;
    //}
    return ret;
}

// returns the position of lsb bit. returns 0 if the parameter is 0.
constexpr size_t lsb_pos(size_t v) noexcept {
    size_t ret = 0;
    for (; v != 0 && (v & 1) == 0; v >>= 1) ret++;
    return ret;
}

template <std::unsigned_integral T>
T gray_decode0(T gray) noexcept {
    // constexpr log2 is one of gcc extension (not standard c++)
    // size_t constexpr cnt = std::log2(sizeof(T) * CHAR_BIT);
    size_t constexpr cnt = lsb_pos(sizeof(T) * CHAR_BIT);
    auto constexpr seq = cpc::sequence_view(
        [](auto const acc) noexcept -> size_t { return acc << 1; }, size_t{1},
        cnt);
    for (auto const n : seq) {
        if (T mask = gray >> n; mask != 0)
            gray ^= mask;
        else
            break;
    }
    return gray;
}

template <std::unsigned_integral T>
std::string to_binary(T const value, size_t const digits) {
    constexpr size_t bitwidth = sizeof(T) * CHAR_BIT;
    return std::bitset<bitwidth>(value).to_string().substr(bitwidth - digits,
                                                           digits);
}

int main(int, char**) {
    std::cout << "Number\tBinary\tGray ,Digit\tDecoded" << std::endl;
    std::cout << "------\t------\t-----,-----\t-------" << std::endl;
    for (auto n : std::ranges::iota_view(0u, 32u)) {
        auto encoded = gray_encode(n);
        auto decoded = gray_decode0(encoded);

        std::cout << n << "\t" << to_binary(n, 5) << "\t"
                  << to_binary(encoded, 5) << ", " << encoded << "\t" << decoded
                  << std::endl;
    }
    std::cout << std::endl;

    // auto lp = lsb_pos(32u);
    // std::cout << "lsb: " << lp << std::endl;

    // gray_decode0(12u);
    return 0;
}
