#include <iostream>
#include "challenge.h"

int main(int, char**) {
    unsigned int upper;
    std::cout << "Num: ";
    std::cin >> upper;

    for (auto i = upper; i > 2; i--) {
        auto result = challenge100::is_prime(i);
        if (result) {
            std::cout << "max prime number: " << i << std::endl;
            break;
        }
    }

    return 0;
}
