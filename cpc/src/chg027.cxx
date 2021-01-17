#include <ranges>

#include "challenge.h"
#include "str_util.h"

void test0() {
    using namespace std::string_literals;
    auto src = "this is,a.. sample!"s;
    auto delim = ",.! "s;
    // coroutine split() の引数は参照型なので、一時オブジェクトを渡すと、co_yield の値を参照する時にはもう destruct されてる
    // auto g0 = cpc::split("this is a sample!"s, " "s);
    auto g0 = cpc::split(src, delim);
    auto r0 = cpc::make_from_generator<std::vector<std::string>>(g0);
    cpc::print_range(r0);
}

int main(int, char**) {
    test0();
    return 0;
}
