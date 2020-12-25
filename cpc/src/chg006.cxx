#include <iostream>

#include "challenge.h"

void print_abundant(int const limit) {
    for (int number = 10; number <= limit; number++) {
        if (auto sum = cpc::sum_proper_divisors(number); sum > number) {
            std::cout << number
                << ", abundance=" << sum - number << std::endl;
        }
    }
}

int main(int, char**) {
    int limit = 0;
    std::cout << "Upper limit: ";
    std::cin >> limit;
    print_abundant(limit);

    return 0;
}
