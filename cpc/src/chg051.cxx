#include <cctype>
#include <algorithm>

#include "challenge.h"


void normalize_dials(std::vector<std::string>& dials, std::string const& country) {
    struct normalizer {
        std::string country;
        normalizer(std::string const& country) 
            : country(country)
        {
        }
        std::string operator()(std::string const& src) {
            constexpr auto valid_chars = [](char const ch) { return std::isdigit(ch) || ch == '+'; };
            auto reg = src | std::views::filter(valid_chars);
            auto dial = std::string(reg.begin(), reg.end());
            //cpc::TypeTracer<decltype(reg)> x;
            //こんな集約関数があればいいのに
            //auto dialtext = src | std::views::filter(valid_chars) | std::views::to<std::string>()
            if (!dial.empty()) {
                if (dial.starts_with('0'))
                    dial = "+" + country + dial.substr(1);
                else if (dial.starts_with(country))
                    dial = "+" + dial;
                else if (dial.starts_with("+" + country))
                    ;
                else
                    dial = "+" + country + dial;
            }
            return dial;
        }
    } n{country};

    std::ranges::transform(std::as_const(dials), dials.begin(), n);
}

void test0() {
    std::vector<std::string> dials{
        "+44744909080",
        "44 7520 112233",
        "+44 7652 12345",
        "40 33492442",
        "7555 23442",
        "044 23423442",
        " 44 23423442",
    };
    using namespace std::string_literals;
    normalize_dials(dials, "44"s);
    for (auto const& dial : dials) {
        std::cout << dial << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}
