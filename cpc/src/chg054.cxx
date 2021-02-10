#include <vector>
#include <iterator>
#include <ranges>
#include <concepts>
#include <utility>

#include "challenge.h"
#include "adjacent_view.h"

void test0() {
    using namespace cpc::stream;
    std::vector v = { 1, 1, 3, 5, 8, 13, 21 };
    //auto adj = cpc::adjacent_view(v, mkpair);
    auto adj = v | cpc::views::adjacent(cpc::mkpair);
    for (auto const& v : adj) {
        std::cout << v << "\n";
    }
}

int main(int, char**) {
    std::cout << std::boolalpha;
    test0();
    return 0;
}
