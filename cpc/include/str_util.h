#pragma once
#include <string>
#include <string_view>
#include <ranges>
#include <concepts>

#include "coro_util.h"

namespace challenge100 {

template <std::integral T, bool EnableRethrow = false>
generator<std::basic_string<T>, EnableRethrow>
split(std::basic_string<T> const& source, T delim) {
    using string_type = std::basic_string<T>;
    for (typename string_type::size_type i = 0, end = source.size(); i < end;) {
        auto sep = source.find(delim, i);
        if (sep == string_type::npos) {
            co_yield string_type{source, i};
            i = end;
        } else {
            co_yield string_type{source, i, sep - i};
            i = sep + 1;
        }
    }
}

template <std::ranges::input_range Container, std::integral V, bool EnableRethrow = false>
requires std::convertible_to<std::ranges::range_value_t<std::ranges::range_value_t<Container>>, V>
cpc::generator<V, EnableRethrow>
join(Container&& c, std::basic_string_view<V> separator) {
    bool isFirst = true;
    for (auto&& token : c) {
        if (!isFirst) {
            for (V c : separator) {
                co_yield c;
            }
        }
        if (isFirst) isFirst = false;
        for (auto&& s : token) {
            co_yield s;
        }
    }
}

template <std::integral T, bool EnableRethrow = false>
std::basic_string<T> string_from_generator(cpc::generator<T, EnableRethrow>& g) {
    return {std::ranges::begin(g), std::ranges::end(g)};
}

} // closing namespace challenge100
