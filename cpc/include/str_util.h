#pragma once
#include <string>
#include <string_view>
#include <ranges>
#include <concepts>

#include "coro_util.h"

namespace challenge100 {

template <std::integral T, bool EnableRethrow = false>
generator<std::basic_string<T>, EnableRethrow>
split(std::basic_string<T> const& source, std::basic_string<T> const& delim) {
    using string_type = std::basic_string<T>;
    std::ranges::range_size_t<string_type> pos, prev_pos = 0;
    while ((pos = source.find_first_of(delim, prev_pos)) != string_type::npos) {
        if (pos > prev_pos) {
            co_yield source.substr(prev_pos, pos - prev_pos);
        }
        prev_pos = pos + 1;
    }
    if (prev_pos < source.length())
        co_yield source.substr(prev_pos, string_type::npos); 
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
std::basic_string<T> string_from_generator(generator<T, EnableRethrow>& g) {
    return make_from_generator<std::basic_string<T>, EnableRethrow>(g);
}

} // closing namespace challenge100
