#include <concepts>
#include <string_view>
#include <vector>
#include "challenge.h"
#include "sequence_view.h"

template <std::integral T>
std::basic_string<T> longest_palindrome(std::basic_string_view<T> str) {
    using str_type = std::basic_string<T>;
    using size_type = std::ranges::range_size_t<str_type>;
    constexpr size_type origin = 0;
    namespace ranges = std::ranges;
    size_type const len = str.size();
    size_type longestBegin = 0, maxLen = 1;
    std::vector<bool> table(len * len, false);

    ranges::for_each(ranges::iota_view{origin, len}, [len,&table](auto n) { table[n * len + n] = true; });
    for (auto i : ranges::iota_view{origin, len - 1}
            | std::views::filter([len, &str](auto n) {
                return str[n] == str[n + 1];
                })) {
        table[i * len + 1 + 1] = true;
        if (maxLen < 2) {
            longestBegin = i;
            maxLen = 2;
        }
    }
    auto seq = cpc::sequence_view([] (auto n) { return ++n; }, static_cast<size_type>(3), static_cast<size_type>(len - 2));
    for (auto k : seq) {
        auto inseq = cpc::sequence_view([] (auto n) { return ++n; }, static_cast<size_type>(0), static_cast<size_type>(len - k + 1));
        for (auto i : inseq) {
            //std::cout << k << "," << i << "\n";
            size_type j = i + k - 1;
            if (str[i] == str[j] && table[(i+1) * len + j - 1]) {
                table[i * len + j] = true;
                if (maxLen < k) {
                    longestBegin = i;
                    maxLen = k;
                }
            }
        }
    }

    return std::basic_string<T>{str.substr(longestBegin, maxLen)};
}

void test0(std::string_view src) {
    auto r0 = longest_palindrome(src);
    std::cout << src << "->" << r0 << std::endl;
}

int main(int, char**) {
    using namespace std::string_view_literals;
    test0("level"sv);
    test0("sahararahnide"sv);
    return 0;
}
