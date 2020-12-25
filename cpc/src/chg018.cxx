#include <iostream>

#include "challenge.h"
#include "minimum.h"

void mintest0() {
    auto res = cpc::minimum(3, 2, 4, -4, 2);
    std::cout << "mintest0: " << res << std::endl;
}

void mintest1() {
    auto res = cpc::minimum(std::less{},
            3, 2, 10, 1);
    std::cout << "mintest1: " << res << std::endl;
}

int main(int, char**) {
    mintest0();
    mintest1();

    return 0;
}
