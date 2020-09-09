#include <iostream>
#include <vector>
#include <iterator>

#include "challenge.h"
#include "sequence_view.h"

void test1() {
    auto fn = [](auto const acc, auto const n) { return acc + n; };
}

void test0() {
    auto constexpr fn = [](auto const n, size_t const)  { return n * 2; };
    auto constexpr seq = cpc::sequence_view(fn, 1, 3);
    auto itr = seq.begin();
    auto end = seq.end();
    std::cout << std::boolalpha;
    std::cout << "sizeof fn: " << sizeof(fn) << std::endl;
    std::cout << "first: " << *itr++ << std::endl;
    std::cout << "second: " << *itr++ << std::endl;
    std::cout << "isNotEnd: " << (itr != end) << std::endl;;
    std::cout << "third: " << *itr++ << std::endl;
    //std::cout << "isEnd: " << std::weakly_incrementable<decltype(itr)>;
    std::cout << "isEnd: " << (itr == end) << std::endl;
    std::cout << std::endl;

    for (auto n : seq) {
        std::cout << n << ",";
    }
    std::cout << std::endl;

}

int main(int, char**) {
    test0();
    return 0;
}
