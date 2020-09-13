#include <iostream>

#include "challenge.h"
#include "ipv4.h"

int main(int, char**) {
    std::cout << "input ipv4 range: ";
    cpc::ipv4 a1, a2;
    std::cin >> a1 >> a2;

    if (a1 < a2) {
        for (; a1 <= a2; ++a1) {
            std::cout << a1 << std::endl;
        }
    } else {
        std::cout << "Invalid range";
    }
    return 0;
}

