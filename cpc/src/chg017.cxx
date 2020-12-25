#include <iostream>

#include "challenge.h"
#include "array2d.h"

template <class T, size_t R, size_t C>
void print_array2d(cpc::array2d<T, R, C> const& ary) {
    std::copy(std::cbegin(ary), std::cend(ary),
            std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::endl;
}

int main(int, char**) {
    cpc::array2d<int, 2, 3> a { 1, 2, 3, 4, 5, 6 };
    for (size_t i = 0; i < a.size(1); ++i) {
        for (size_t j = 0; j < a.size(2); ++j) {
            a(i, j) *= 2;
        }
    }

    // iteration
    print_array2d(a);


    // fill
    decltype(a) b;
    b.fill(1);
    print_array2d(b);

    a.swap(b);
    print_array2d(b);


    decltype(a) c(std::move(b));
    print_array2d(c);


    return 0;
}
