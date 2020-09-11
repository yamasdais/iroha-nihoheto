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

template <std::ranges::range Range, std::unsigned_integral Num = std::ranges::range_value_t<Range>>
std::tuple<Num, Num, size_t> longest_collats(Range&& range) {
    constexpr bool isVerbose = false;
    size_t length = 0;
    Num number = 0;
    Num record = 0;
    //std::vector<int> len_cache(range.size() + 1, 0);

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
        auto seq = cpc::accum(collats, std::pair<size_t, Num>{},
                [](auto&& acc, auto const n) {
                    if constexpr (isVerbose)
                        std::cout << n << ",";
                    acc.first++;
                    if (acc.second < n)
                        acc.second = n;
                    return acc;
                });
#endif
        if (length < seq.first) {
            length = seq.first;
            number = seq.second;
            record = first_term;
        }
        if constexpr (isVerbose) {
            auto [l, mx] = seq;
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
