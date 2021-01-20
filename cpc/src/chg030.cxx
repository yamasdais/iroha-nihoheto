#include <regex>
#include <optional>

#include "challenge.h"

struct uri_parts {
    std::string protocol;
    std::string domain;
    std::optional<int> port;
    std::optional<std::string> path;
    std::optional<std::string> query;
    std::optional<std::string> fragment;

    void print(std::ostream& out) {
        out << "Protocol: " << protocol << "\n";
        out << "Domain: " << domain << "\n";
        out << "Port: " << port.value_or(-1) << "\n";
        out << "Path: " << path.value_or("") << "\n";
        out << "Query: " << query.value_or("") << "\n";
        out << "Fragment: " << fragment.value_or("") << "\n";
    }
};

std::optional<uri_parts> parse_uri(std::string const& uri) {
    std::regex re(R"((\w+):\/\/([\w.-]+)(?::(\d{1,5}))?)"
                  R"(([\w\.\/]+)?(?:\?([\w=&%]*))?(?:#([\w]+))?)");
    auto matcher = std::smatch{};
    return cpc::make_optional_if(
            std::regex_match(uri, matcher, re)
            && matcher[1].matched
            && matcher[2].matched,
            [&matcher]() {
                return uri_parts{
                    { matcher[1].str() },
                    { matcher[2].str() },
                    cpc::make_optional_if(matcher[3].matched, [&matcher]() { return std::stoi(matcher[3].str()); }),
                    cpc::make_optional_if(matcher[4].matched, [&matcher]() { return matcher[4].str(); }),
                    cpc::make_optional_if(matcher[5].matched, [&matcher]() { return matcher[5].str(); }),
                    cpc::make_optional_if(matcher[6].matched, [&matcher]() { return matcher[6].str(); })
                    };
                });
}

void test0(std::string const& uri) {
    auto result = parse_uri(uri);
    if (result) {
        result->print(std::cout);
    }
}

int main(int, char**) {
    test0("http://foo.bar.com:8080/baz/quux?abc=foo&bb=42#entry");
    test0("http://github.com/yamasdais");
    return 0;
}
