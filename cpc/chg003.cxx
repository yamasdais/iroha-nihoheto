#include <iostream>
#include <list>
//#include <format>
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
    using namespace std;
    //auto result = accumulate(begin(nums), end(nums), 1u, challenge100::lcm<decltype(nums)::value_type>);
    auto result = challenge100::lcmr(begin(nums), end(nums));
    std::cout << "LCM: " << result << std::endl;;
}

int main(int, char**) {
    doCalc();

    return 0;
}
