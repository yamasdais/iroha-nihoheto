#include <iostream>

#include "challenge.h"
#include "ipv4.h"

int main(int, char**) {
    cpc::ipv4 address{192, 168, 0, 1};
    std::cout << address << std::endl;

    cpc::ipv4 ip;
    std::cout << ip << std::endl;
    std::cin >> ip;
    if (!std::cin.fail()) {
        std::cout << ip << std::endl;
    }

    return 0;
}

