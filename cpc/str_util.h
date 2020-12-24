#include <string>
#include <string_view>
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

} // closing namespace challenge100
