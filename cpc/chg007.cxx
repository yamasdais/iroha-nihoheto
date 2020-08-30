#include <iostream>

#include "challenge.h"

void print_amicables(int const limit) {
    for (int num = 4; num < limit; num++) {
        if (auto sum1 = cpc::sum_proper_divisors(num); sum1 < limit) {
            if (auto sum2 = cpc::sum_proper_divisors(sum1);
                    sum2 == num && sum1 > sum2) {
                std::cout << num << "," << sum1 << std::endl;
            }
        }
    }
}

int main(int, char**) {
    int limit = 0;
    std::cout << "Upper limit: ";
    std::cin >> limit;

    print_amicables(limit);

    return 0;
}

