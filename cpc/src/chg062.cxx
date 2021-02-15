#include <iostream>
#include <vector>
#include <algorithm>
#include <concepts>
#include <ranges>
#include <thread>
#include <random>

#include "challenge.h"

template <std::ranges::input_range Range,
          std::regular_invocable<std::ranges::iterator_t<Range>,
              std::ranges::iterator_t<Range>> Func>
auto pprocess(Range const& input, Func&& func) {
    std::vector<std::thread> threads;
    int thread_count = std::thread::hardware_concurrency();
    std::vector<std::iter_value_t<Range>> candidates(thread_count);
    auto chunk_size = std::size(input) / thread_count;
    auto last = std::ranges::begin(input);

    for (int i = 0; i < thread_count; ++i) {
        auto first = std::move(last);
        if (i == thread_count - 1)
            last = std::ranges::end(input);
        else
            std::advance(last, chunk_size);

        threads.emplace_back([first, last, &func, &r=candidates[i]]() {
                r = std::invoke(std::forward<Func>(func), first, last);
                });
    }
    std::ranges::for_each(threads, [](auto& t) {
            t.join();
            });
    return std::invoke(std::forward<Func>(func),
            std::ranges::begin(candidates), std::ranges::end(candidates));
}

template <std::ranges::input_range Range>
auto pmin(Range const& input) {
    return pprocess(input, [](auto b, auto e) {
            return *std::min_element(b, e);
            });
}

template <std::ranges::input_range Range>
auto pmax(Range const& input) {
    return pprocess(input, [](auto b, auto e) {
            return *std::max_element(b, e);
            });
}

void test0() {
    constexpr size_t count = 1'000'000;
    std::vector<int> data(count);
    std::random_device rd;
    std::mt19937 mt;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::ranges::generate(seed_data, std::ref(rd));
    std::seed_seq seq(std::ranges::begin(seed_data), std::ranges::end(seed_data));
    mt.seed(seq);
    std::uniform_int_distribution<> ud(1, 100);

    std::ranges::generate_n(std::ranges::begin(data), count, [&mt, &ud]() { return ud(mt); });

    auto rmin = cpc::perf_timer<std::chrono::milliseconds>::duration([&data]() {
            return pmin(data); });
    std::cout << "Min: " << get<0>(rmin) << "(" << get<1>(rmin).count() << "ms)\n";

    auto rmax = cpc::perf_timer<std::chrono::milliseconds>::duration([&data]() {
            return pmax(data); });
    std::cout << "Max: " << get<0>(rmax) << "(" << get<1>(rmax).count() << "ms)\n";
}

int main(int, char**) {
    test0();

    return 0;
}
