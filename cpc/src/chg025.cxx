#include <concepts>
#include <cctype>
#include <string_view>

#include "challenge.h"
#include "coro_util.h"

template <std::integral T>
cpc::generator<T> capitalize(std::basic_string_view<T> src) {
    bool newWord = true;
    for (auto const ch : src) {
        newWord = newWord || std::ispunct(ch) || std::isspace(ch);
        co_yield std::isalpha(ch)
            ? static_cast<T>(newWord ? (newWord = false, std::toupper(ch)) : std::tolower(ch))
            : ch;
    }
}

template <std::integral T>
void do_caps(std::basic_string_view<T> src) {
    auto gen = capitalize(src);
    auto text = std::string{gen.begin(), gen.end()};
    std::cout << "text: " << text << std::endl;
}

int main(int, char**) {
    using namespace std::string_view_literals;
    do_caps("the c++ challenger"sv);
    return 0;
}
