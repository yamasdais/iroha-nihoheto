#include <iostream>
#include <list>
//#include <format>
#include <ranges>
#include <numeric>
#include "challenge.h"

auto getNumber(int order) {
    unsigned int ret;
    std::cout << "Number[" << order << "]: ";
    std::cin >> ret;

    return ret;
}

auto doCalc() {
    auto nums = std::list<unsigned int>{};

    unsigned int num;
    int order = 0;
    std::cout << "(enter 0 to exit)" << std::endl;
    while (true) {
        num = getNumber(order++);
        if (num == 0) break;
        nums.emplace_back(num);
    }
    if (std::size(nums) <= 1) {
        std::cout << "not enough number" << std::endl;
        return;
    }
    auto result = challenge100::lcmr(std::ranges::cbegin(nums), std::ranges::cend(nums));
    std::cout << "LCM: " << result << std::endl;;
}

int main(int, char**) {
    doCalc();

    return 0;
}
