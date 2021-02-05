#include <algorithm>
#include <ranges>

#include "challenge.h"

void print_permutations(std::string str) {
    std::ranges::sort(str);
    do {
        std::cout << str << "\n";
    } while (std::ranges::next_permutation(str).found);
}

void next_permutation(std::string str, std::string perm = {}) {
    if (str.empty())
        std::cout << perm << "\n";
    else
        std::ranges::for_each(std::views::iota(0u, str.size()), [&]([[maybe_unused]] auto const i) {
                next_permutation(str.substr(1), perm + str[0]);
                std::ranges::rotate(str, std::ranges::begin(str) + 1);
                });
}

void test0(std::string const& str) {
    std::cout << "ranges::permutation" << "\n";
    print_permutations(str);

    std::cout << "recursive permutation" << "\n";
    next_permutation(str);

    std::cout << "Done." << std::endl;
}

int main(int, char**) {
    using namespace std::string_literals;
    test0("main"s);
    return 0;
}
