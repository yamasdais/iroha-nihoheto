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
auto longest_collats0(Range&& range) {
    constexpr bool isVerbose = true;
    size_t max_length = 0;
    Num number = 0;
    std::vector<int> len_cache(range.size() + 1, 0);
    len_cache[1] = 1;

    for (auto first_term : range) {
        auto collats = collats_sequence(first_term);
        Num last = 0;
        auto seq = collats | std::ranges::views::take_while([first_term, &last](auto num) {
                if (first_term <= num)
                    return true;
                last = num;
                return false;
                });
        auto len = cpc::accum(seq, size_t{}, [](auto&& acc, auto const n) {
                if constexpr (isVerbose)
                    std::cout << n << ",";
                return acc + 1;
                });
        if (last < first_term) {
            if constexpr (isVerbose)
                std::cout << "...";
            len += len_cache[last];
        }
        len_cache[first_term] = len;

        if constexpr (isVerbose) {
            std::cout << "\nlast: " << last <<  ", len: " << len << "(+" << len_cache[last] << ")" << std::endl;
        }
        if (len > max_length) {
            number = first_term;
            max_length = len;
        }
    }

    return std::make_pair(number, max_length);
}

int main(int, char**) {
    unsigned long long start = 1;
    unsigned long long limit = 27;
    std::cout << std::boolalpha;

    auto [term, len] = longest_collats0(std::ranges::views::iota(start, limit + 1));
    std::cout << "longest term: " << term << ", len: " << len << std::endl;
    return 0;
}
