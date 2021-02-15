#include <iostream>
#include <algorithm>
#include <chrono>
#include <concepts>
#include <ranges>
#include <future>
#include <random>

#include "challenge.h"

template <std::ranges::input_range Range,
          std::regular_invocable<std::ranges::iterator_t<Range>,
              std::ranges::iterator_t<Range>> Func>
auto asprocess(Range const& input, Func&& func) {
    std::vector<std::future<std::iter_value_t<Range>>> tasks;
    int thread_count = std::thread::hardware_concurrency();
    auto chunk_size = std::size(input) / thread_count;
    auto last = std::ranges::begin(input);

    for (int i = 0; i < thread_count; ++i) {
        auto first = std::move(last);
        if (i == thread_count - 1)
            last = std::ranges::end(input);
        else
            std::advance(last, chunk_size);

        tasks.emplace_back(std::async(std::launch::async,
                    [first, last, f=std::forward<Func>(func)]() {
                    return std::invoke(f, first, last);
                    }));
    }
    std::vector<std::iter_value_t<Range>> candidates(thread_count);
    std::ranges::for_each(tasks, [&candidates](auto& t) {
            candidates.push_back(t.get());
            });

    return std::invoke(std::forward<Func>(func),
            std::ranges::begin(candidates), std::ranges::end(candidates));
}

template <std::ranges::input_range Range>
auto pmin(Range const& input) {
    return asprocess(input, [](auto b, auto e) {
            return *std::min_element(b, e);
            });
}

template <std::ranges::input_range Range>
auto pmax(Range const& input) {
    return asprocess(input, [](auto b, auto e) {
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

/*
 * 非同期版は、thread 版と比べると少し遅いかな、という感じ
 *
 */
