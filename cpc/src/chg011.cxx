#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <ranges>

std::string to_roman(unsigned int value) {
    std::vector<std::pair<unsigned int, char const*>> const roman {
        { 1000, "M" }, { 900, "CM" }, { 500, "D" }, { 400, "CD" },
        { 100, "C" }, { 90, "XC" }, { 50, "L" }, { 40, "XL" },
        { 10, "X" }, { 9, "IX" }, { 5, "V" }, { 4, "IV" }, { 1, "I" }
    };
    std::string result;
    for (auto const& [num, str] : roman) {
        while (value >= num) {
            result += str;
            value -= num;
        }
    }

    return result;
}

int main(int, char**) {
    for (auto n : std::ranges::iota_view(1u, 101u)) {
        std::cout << n << "\t" << to_roman(n) << std::endl;
    }

    int number = 0;
    std::cout << "number: ";
    std::cin >> number;
    std::cout << to_roman(number) << std::endl;

    return 0;
}
