#include <iostream>

#include "challenge.h"

int main(int, char**) {
    unsigned long upper;
    std::cout << "Upper limit: ";
    std::cin >> upper;

    for (auto i = upper; i > 1; i--) {
        if (cpc::is_prime(i)) {
            std::cout << "max prime number: " << i << std::endl;
            break;
        }
    }

    return 0;
}
