#include <algorithm>
#include <iomanip>
#include <numeric>
#include <concepts>
#include <ranges>

#include "challenge.h"
#include "functional.h"

template <std::ranges::bidirectional_range Range>
requires (std::ranges::sized_range<Range>)
double trim_mean(Range values, double const trim_ratio = 0.05) {
    std::ranges::sort(values);
    auto const trim_cnt = static_cast<size_t>(values.size() * trim_ratio + 0.5);
    auto const trim_size = values.size() - trim_cnt * 2;
    if (trim_size == 0)
        return 0.0;
    //auto total = std::accumulate(std::ranges::begin(values) + trim_cnt, std::ranges::end(values) - trim_cnt, 0ul);
    auto const total = cpc::accum(std::ranges::begin(values)+trim_cnt, std::ranges::end(values)-trim_cnt,
            0ul, [](auto const sum, auto const v) { return sum + v; });
    return static_cast<double>(total) / trim_size;
}

struct movie {
    int id;
    std::string title;
    std::vector<int> ratings;
};

void print_movie_ratings(std::vector<movie> const& movies) {
    for (auto const& m : movies) {
        std::cout << m.title << ": "
            << std::fixed << std::setprecision(1)
            << trim_mean(m.ratings) << "\n";
    }
}

void test0() {
    std::vector<movie> movies {
        { 101, "The Matrix", { 10, 9, 10, 9, 8, 7, 10, 5, 9, 9, 9, 8} },
        { 102, "Gladiator", { 10, 5, 7, 8, 9, 10, 10, 6, 7, 8, 7, 7, 6, 5, 8} },
        { 103, "Interstellar", { 10, 10, 10, 9, 3, 8, 8, 9, 7, 4, 10, 8, 6} },
    };
    print_movie_ratings(movies);
}

int main(int, char**) {
    test0();
    return 0;
}
