#include "challenge.h"
#include "zip_view.h"

void test0() {
    using namespace cpc::stream;
    std::vector const va = { 0, 10, 20, 30 };
    std::vector<double> const vb = { 3.14, 4.24, 6.843 };

    auto zv = cpc::zip_view{va, vb, cpc::mkpair};
    auto itr = std::ranges::begin(zv);
    auto end = std::ranges::end(zv);
    auto v = *itr;
    std::cout << v << "\n";
    ++itr;
    std::cout << *itr << "\n";
    ++itr;
    std::cout << *itr << "\n";
    ++itr;
    std::cout << "end?: " << (itr == end) << "\n";
    for (auto const& vpair : zv) {
        std::cout << vpair << "\n";
    }
    std::cout << "views test." << "\n";
    auto viewzip = vb | cpc::views::zip(va, cpc::mkpair);
    auto vzitr = std::ranges::begin(viewzip);
    auto vzend = std::ranges::end(viewzip);
    std::cout << "end?: " << (vzitr == vzend) << "\n";
    std::cout << "same?: " << (vzitr == std::ranges::iterator_t<decltype(viewzip)>{}) << "\n";
    std::cout << *vzitr << "\n";
    std::cout << *++vzitr << "\n";
    while (vzitr != vzend) {
        std::cout << *vzitr << "\n";
        ++vzitr;
    }
    for (auto const& vpair : viewzip) {
        std::cout << vpair << "\n";
    }
}

int main(int, char**) {
    std::cout << std::boolalpha;
    test0();

    return 0;
}
