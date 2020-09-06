#include <iostream>
#include <iterator>
#include <algorithm>

#include "challenge.h"

// 素因数分解

int main(int, char**) {
    using c9type = unsigned long long;
    c9type number = 0u;
    std::cout << "number: ";
    std::cin >> number;

    auto factors = cpc::prime_factors(number);
    std::copy(std::cbegin(factors), std::cend(factors),
              std::ostream_iterator<c9type>{std::cout, " "});

    return 0;
}
