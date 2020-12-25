#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <cctype>

#include "challenge.h"
#include "sequence_view.h"

// ISBN check

// ISBN-10
bool validate_isbn10(std::string_view isbn) {
    constexpr bool isVerbose = true;
    bool first = true;
    auto f = [&first](char const c) {
        if (first == true) {
            first = false;
            return std::isdigit(c)
               ? c - '0'
                   : (c == 'x' || c == 'X')
                   ? 10 : -1;
        }
        return std::isdigit(c) ? c - '0' : -1;
    };
    auto seq = isbn
             | std::views::reverse
             | std::views::transform(f)
             | std::views::filter([](auto const n) { return n >= 0; });
    auto i = 0;
    auto sum = cpc::accum(seq, 0u,
            [&i](auto acc, auto c) {
            auto plus = ++i * c; 
            if (isVerbose)
                std::cout << c << ": " << acc << ", " << i << "(+" << plus << ")" << std::endl;
            return acc + plus;
            });

    if (isVerbose)
        std::cout << "\nlen: " << i << ", sum: " << sum << std::endl;

    return i == 10 && !(sum % 11);
}

bool validate_isbn13(std::string_view isbn) {
    constexpr bool isVerbose = true;
    auto seq = isbn
        | std::views::reverse
        | std::views::filter([](auto const c) { return std::isdigit(c); })
        | std::views::transform([](auto const c) { return c - '0'; });
    auto x = false;
    auto len = 0u;
    auto sum = cpc::accum(seq, 0u,
            [&x, &len](auto acc, auto n) {
            auto plus = (x ? 3 : 1) * n; 
            if (isVerbose)
                std::cout << n << ": " << acc << ", " << x << "(+" << plus << ")" << std::endl;
            x = !x;
            len++;
            return acc + plus;
            });
    if (isVerbose)
        std::cout << "\nlen: " << len << ", sum: " << sum << std::endl;

    return len == 13 && !(sum % 10);
}

int main(int, char**) {
    //validate_isbn("4101092052");
    std::string code10 = "4101092052";
    char const* code13 = "978-4-86410-690-0";

    auto result = validate_isbn10(code10);
    std::cout << "code: " << code10 << " is "
        << (result ? "valid" : "invalid") << " ISBN code" << std::endl;

    std::cout << std::boolalpha;
    result = validate_isbn13(code13);
    std::cout << "code: " << code13 << " is "
        << (result ? "valid" : "invalid") << " ISBN code" << std::endl;

    return 0;
}

