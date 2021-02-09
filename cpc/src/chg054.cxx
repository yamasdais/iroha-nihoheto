#include <vector>
#include <iterator>
#include <ranges>
#include <concepts>
#include <utility>

#include "challenge.h"
#include "adjacent_view.h"

namespace challenge100 {
namespace stream {
template <class T0, class T1>
std::ostream& operator<<(std::ostream& out, std::pair<T0, T1> const& p) {
    out << "(" << p.first << "," << p.second << ")";
    return out;
}

} // challenge100::stream

}

constexpr auto mkpair = [](auto const fst, auto const snd) {
    return std::make_pair(fst, snd);
};


void test0() {
    using namespace cpc::stream;
    std::vector v = { 1, 1, 3, 5, 8, 13, 21 };
    //auto adj = cpc::adjacent_view(v, mkpair);
    auto adj = v | cpc::views::adjacent(mkpair);
    for (auto const& v : adj) {
        std::cout << v << "\n";
    }
}

int main(int, char**) {
    std::cout << std::boolalpha;
    test0();
    return 0;
}
