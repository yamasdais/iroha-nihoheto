#include <list>
#include <vector>
#include <ranges>

#include "challenge.h"

template <class C, class T = std::ranges::range_value_t<C>>
concept has_pushback = requires(C&& container, T&& value) {
    container.push_back(value);
};

template <class C, class... T>
requires (has_pushback<C, T> && ...)
void push_back(C& c, T&& ...arg) {
    (c.push_back(arg), ...);
}

int main(int, char**) {
    std::vector<int>  v;
    push_back(v, 3, 5 ,7);
    cpc::print_range(v);

    std::list<int> l;
    push_back(l, 2, 4, 6, 8);
    cpc::print_range(l);

    return 0;
}
