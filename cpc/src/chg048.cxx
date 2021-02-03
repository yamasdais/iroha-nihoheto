#include <utility>
#include <vector>
#include <map>
#include <ranges>
#include <algorithm>
#include <concepts>

#include "challenge.h"
#include "coro_util.h"
#include "functional.h"

template <std::ranges::input_range Container>
cpc::generator<std::pair<std::ranges::range_value_t<Container>, size_t>>
find_most_frequent(Container const& c) {
    using value_type = std::ranges::range_value_t<Container>;
    using map_type = std::map<value_type, size_t>;
    using map_value_type = typename map_type::value_type;
    map_type counts;

    for (auto const& elem : c)
        counts[elem]++;

    //auto max_elem = std::ranges::max_element(counts, std::less<size_t>{}, &map_value_type::second);
    // C++20 的には↑の様に書けばいいが、make_projection を動作確認がてら使ってみる
    auto const proj = cpc::make_projection<map_value_type const&, map_value_type const&>(
            std::less<size_t>{}, &map_value_type::second);
    auto const max_elem = std::ranges::max_element(counts, proj);

    auto const filter_proj = cpc::make_projection<map_value_type const&>([max_cnt=max_elem->second](size_t cnt) {
            return cnt == max_cnt;
            }, &map_value_type::second);
    // C++20 的には↓の様に書けばいいが、make_projection を動作確認がてら使ってみる
    // for ループの記述がスッキリするので、lambda を伴うならこっちのほうが良いかな
#if 0
    for (auto const& elem : counts | std::views::filter(
                [mc=max_elem->second](auto const& e) {
                    return e.second == mc;
                })) {
#endif
    for (auto const& elem : counts | std::views::filter(filter_proj)) {
        co_yield elem;
    }
}

void test0() {
    auto range = std::vector{ 1, 2, 3, 5, 8, 13, 3, 5, 8, 8, 5 };
    for (auto const& [elem, count] :  find_most_frequent(range)) {
        std::cout << elem << ":" << count << std::endl;;
    }
}

int main(int, char**) {
    test0();
    return 0;
}
