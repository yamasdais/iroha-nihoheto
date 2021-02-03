#include <cctype>
#include <map>
#include <iomanip>
#include <string_view>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <utility>

#include "challenge.h"
#include "functional.h"

std::map<char, double> text_histogram(std::string_view text) {
    std::map<char, double> freq;
    using mapval_type = std::map<char, double>::value_type;
    constexpr auto f_alpha = [](char const ch) { return std::isalpha(ch); };

    //for (auto ch : text | std::views::filter(f_alpha)) {
    //    freq[ch]++;
    //}
    std::ranges::for_each(text | std::views::filter(f_alpha), [&freq](char ch) { freq[std::tolower(ch)]++; });
    auto const total = cpc::accum(std::as_const(freq), 0ul, [](auto const sum, auto const cnt) {
                return sum + static_cast<unsigned long>(cnt);
            }, &mapval_type::second);
    std::ranges::for_each(freq, [total](auto& cnt) { cnt = (100.0 * cnt) / total; }, &mapval_type::second);

    return freq;
}

void test0() {
    auto result = text_histogram(R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit, )"
            R"(sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.)");
    for (auto const& [ch, rate] : result) {
        std::cout << ch << " : "
            << std::fixed << std::setw(5) << std::setfill(' ')
            << std::setprecision(2) << rate << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}
