#include <execution>

#include "challenge.h"

template <std::ranges::input_range Range, std::equality_comparable T>
bool contains(Range const& c, T&& value) {
    return std::ranges::cend(c) != std::find(std::execution::par_unseq,
             std::ranges::cbegin(c), std::ranges::cend(c), std::forward<T>(value));
}

template <std::ranges::input_range Range, std::equality_comparable... T>
bool contains_any(Range const& c, T&&... vals)
{
    return (... || contains(c, std::forward<T>(vals)));
}

template <std::ranges::input_range Range, std::equality_comparable... T>
bool contains_all(Range const& c, T&&... vals)
{
    return (... && contains(c, std::forward<T>(vals)));
}

template <std::ranges::input_range Range, std::equality_comparable... T>
bool contains_none(Range const& c, T&&... vals)
{
    return !contains_any(c, std::forward<T>(vals)...);
}

int main(int, char**) {
    std::vector<int> ary{ 3, 5, 7, 9, 11, 13 };
    std::cout << std::boolalpha;

    std::cout << "any found?: " << contains_any(ary, 2, 5) << std::endl;
    std::cout << "all found?: " << contains_all(ary, 3, 5) << std::endl;
    std::cout << "none found?: " << contains_none(ary, 2, 4) << std::endl;
    return 0;
}
