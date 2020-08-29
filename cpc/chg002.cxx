#include <iostream>

#include "challenge.h"

int main(int, char**) {
    unsigned int fst, snd;
    std::cout << "Num 1st: ";
    std::cin >> fst;
    std::cout << "Num 2nd: ";
    std::cin >> snd;

    auto result = cpc::gcd(fst, snd);
    std::cout << "GCD: " << result << std::endl;
    ;
    return 0;
}
