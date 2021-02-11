#include "challenge.h"
#include "quicksort.h"

void test0() {
    std::vector v0 { 3, 4, 1, 3, 6, 9, 12 };
    cpc::print_range(v0);
    std::cout << "\n";
    cpc::quicksort(v0, std::ranges::greater{});
    cpc::print_range(v0);
    std::cout << "\n";
}

int main(int, char**) {
    std::cout << std::boolalpha;
    test0();

    return 0;
}
