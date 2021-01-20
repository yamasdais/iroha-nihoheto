#include <regex>

#include "challenge.h"

std::string transform_date(std::string const& src) {
    std::regex re{ R"((\d{1,2})[-\.\/](\d{1,2})[-\.\/](\d{4}))"};
    return std::regex_replace(src, re, "$3-$2-$1");
}

void test0(std::string const& src) {
    auto rep = transform_date(src);
    std::cout << src << " => " << rep << std::endl;
}

int main(int, char**) {
    using namespace std::string_literals;
    test0("21-01-2021");
    test0("21/1/2021");
    test0("3.1.2021");

    return 0;
}
