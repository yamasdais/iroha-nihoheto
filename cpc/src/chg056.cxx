#include <iostream>
#include <vector>
#include <concepts>
#include <ranges>

#include "challenge.h"

template <std::ranges::input_range V, std::copy_constructible F>
auto select(V&& range, F&& func) {
    return std::forward<V>(range) | std::views::transform(std::forward<F>(func));
}
struct book {
    int id;
    std::string title;
    std::string author;
};

void test0() {
    std::vector vec = { 0, 2, 4, 6, 8 };
    auto add1 = [](auto const v) { return v + 1; };
    auto r = select(vec, add1);
    std::vector result(std::ranges::begin(r), std::ranges::end(r));
    std::ranges::for_each(result, [](auto const v) { std::cout << v << "\n"; });
    std::cout << std::endl;
    //cpc::TypeTracer<decltype(result)> x;
    for (auto const v : vec | std::views::transform(add1)) {
        //cpc::TypeTracer<decltype(*v)> x;
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void test1() {
    std::vector<book> books {
        { 100, "The C Programming Language", "Bjarne Stroustrup" },
        { 101, "Effective Modern C++", "Scott Mayers" },
        { 102, "The Modern C++ Challenge", "Marius Bancila" },
    };
    for (auto const& title : select(books, &book::title)) {
        std::cout << title << ",";
    }
    std::cout << std::endl;
}

int main(int, char**) {
    test0();
    test1();

    return 0;
}
