#include <iostream>

#include "challenge.h"
#include "ipv4.h"

int main(int, char**) {
    std::cout << std::boolalpha;
    constexpr cpc::ipv4 address{192, 168, 0, 1};
    std::cout << address << std::endl;

    cpc::ipv4 ip;
    std::cout << ip << std::endl;

    auto cmp = (ip < address) > 0;
    std::cout << "cmp: " << cmp << std::endl;
    std::cout << "eq: " << (ip == address) << std::endl;
    std::cout << "ne: " << (ip != address) << std::endl;
    std::cout << "le: " << (ip < address) << std::endl;
    std::cout << "ge: " << (ip > address) << std::endl;

    std::cin >> ip;
    ++ip;
    if (!std::cin.fail()) {
        std::cout << ip << std::endl;
    }

    return 0;
}

