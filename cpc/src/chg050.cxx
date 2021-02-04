#include <regex>
#include <algorithm>
#include <concepts>

#include "challenge.h"
#include "coro_util.h"

template <std::ranges::forward_range Range>
    requires std::convertible_to<std::iter_value_t<Range>, std::string>
cpc::generator<std::string, false>
filter_dial_country(Range const& range, std::string const& countryCode) {
    std::regex re(std::string(R"(^\s*\+?)") + countryCode);
    auto const filter = std::views::filter([&re](auto const& dial) { return std::regex_search(dial, re); });

    for (auto const& dial : range | filter) {
        co_yield dial;
    }
}

void test0() {
    std::vector<std::string> dials{
        "+40744909080",
        "44 7520 112233",
        "+44 7652 12345",
        "40 33492442",
        "7555 23442",
        "044 23423442",
        " 44 23423442",
    };
    auto result = filter_dial_country(dials, "44");

    for (auto const& dial : result) {
        std::cout << dial << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}
