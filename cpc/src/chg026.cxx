#include <ranges>
#include <string_view>

#include "challenge.h"
#include "coro_util.h"
#include "str_util.h"

void test0() {
    using namespace std::string_view_literals;
    std::vector<std::string> words = { "This", "is", "an", "example" };
    auto g0 = cpc::join(words, " "sv);
    //auto s0 = std::string(std::begin(g0), std::end(g0));
    auto s0 = cpc::string_from_generator(g0);

    std::cout << "text: '" << s0 << "'" << std::endl;
}

int main(int, char**) {
    test0();
    return 0;
}
