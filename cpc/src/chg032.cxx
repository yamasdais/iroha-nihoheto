#include <iomanip>
#include <cmath>
#include <ranges>

#include "challenge.h"

unsigned int number_of_digits(unsigned int const i) {
    return i > 0 ? static_cast<int>(std::log10(static_cast<double>(i)) + 1) : 1;
}

void print_pascal_triangle(unsigned int const n) {
    for (auto i : std::ranges::views::iota(0u, n)) {
        auto x = 1;
        std::cout << std::string( (n - i - 1) * (n / 2), ' ' );

        for (int j{0}; j < i; j++) {
            auto y = x;
            x = x * (i - j) / (j + 1);
            auto maxLen = number_of_digits(x) - 1;
            std::cout << y << std::string(n - 1 - maxLen - n % 2, ' ');
        }
        std::cout << "\n";
    }
}

int main(int, char**) {
    print_pascal_triangle(10);
    return 0;
}
