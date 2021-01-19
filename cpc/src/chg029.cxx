#include <concepts>
#include <string_view>
#include <list>
#include <regex>

#include "challenge.h"
#include "coro_util.h"

bool validate_license_plate_format(std::string const& str) {
    std::regex re(R"([A-Z]{3}-[A-Z]{2} [0-9]{3,4})");
    return std::regex_match(str, re);
}

cpc::generator<std::string> select_license_place_format(std::string const& str) {
    std::regex re(R"([A-Z]{3}-[A-Z]{2} [0-9]{3,4})");
    //std::regex re(R"(\d+)");

    for (std::sregex_iterator i(std::ranges::begin(str), std::ranges::end(str), re), end;
           i != end; ++i) {
        //std::cout << i->str() << std::endl;
        co_yield i->str();
    }
}

void test0(std::string const& str) {
    bool res = validate_license_plate_format(str);
    std::cout << str << ": " << res << std::endl;
}

void test1(std::string const& str) {
    auto g = select_license_place_format(str);
    auto lst = cpc::make_from_generator<std::list<std::string>>(g);
    cpc::print_range(lst);
}

int main(int, char**) {
    using namespace std::string_literals;
    std::cout << std::boolalpha;
    test0("ABC-DE 123"s);
    test1("ABC-DE 123 qe-m, ABC-DE 234"s);

    return 0;
}
