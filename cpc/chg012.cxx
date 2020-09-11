#include <iostream>
#include <utility>
#include <vector>
#include <ranges>
#include <concepts>

#include "challenge.h"
#include "sequence_view.h"

// コラッツ数列

template <std::unsigned_integral Num>
constexpr auto collats_sequence(Num const first_term) {
    auto constexpr predEnd = [](Num n) { return n != 0; };
    return cpc::sequence_view(cpc::collats_sequence_fn<Num>{}, first_term)
           | std::views::take_while(predEnd);
}

template <class Num>
using AccT = std::tuple<size_t, Num, bool>;
template <class Num>
struct AccTPred {
    bool operator()(AccT<Num>&& acc) {
        //return std::get<2>(acc);
        return false;
    }
};

template <std::ranges::range Range, std::unsigned_integral Num = std::ranges::range_value_t<Range>>
std::tuple<Num, Num, size_t> longest_collats(Range&& range) {
    constexpr bool isVerbose = true;
    size_t length = 0;
    Num number = 0;
    Num record = 0;
    std::vector<int> len_cache(range.size() + 1, 0);
    //std::cout << "predicate: "
    //    << std::same_as<bool, std::invoke_result_t<AccTPred<Num>, AccT<Num>>> << std::endl;

    for (auto first_term : range) {
        auto collats = collats_sequence(first_term);
#if 0
        // operator| 効かない
        auto seq = collats | cpc::accum(std::pair<size_t, Num>{}, [](auto&& acc, auto const n) {
                acc.first++;
                acc.second = std::max(acc.second, n);
                return acc;
                });
#else
        // workaround
        auto [l, mx, flg] = cpc::accum(collats, AccT<Num>{ 0, 0, true },
                [first_term, &len_cache](auto&& acc, auto const n) {
                    if constexpr (isVerbose)
                        std::cout << n << ",";
                    if (first_term > n) {
                        len_cache[first_term] = std::get<0>(acc) += len_cache[n];
                        std::get<2>(acc) = false; // break accum
                        std::cout << "...";
                    } else {
                        std::get<0>(acc)++;
                        if (first_term == 1)
                            len_cache[1] = std::get<0>(acc);
                    }
                    if (std::get<1>(acc) < n)
                        std::get<1>(acc) = n;
                    return acc;
                }, {}, [](AccT<Num> const& r) { return std::get<2>(r); });
#endif
        if (length < l) {
            length = l;
            number = mx;
            record = first_term;
        }
        if constexpr (isVerbose) {
            std::cout << "\nlen: " << l << ", max: " << mx << std::endl;
        }
    }

    return std::make_tuple(record, number, length);
}

int main(int, char**) {
    unsigned long long start = 1;
    unsigned long long limit = 27;
    std::cout << std::boolalpha;

    //longest_collats(10u);
    auto [term, number, len] = longest_collats(std::ranges::views::iota(start, limit + 1));
    std::cout << "\nCollats range: [" << start << "," << limit << "]\n"
        << "longest term: " << term << ", len: " << len << ", max: " << number << std::endl
        << std::endl;
    return 0;
}
