#include <thread>
#include <concepts>
#include <ranges>
#include <random>
#include <vector>
#include <array>
#include <execution>

#include <algorithm>

#include "challenge.h"

// 普通の transform
template <std::ranges::input_range Range, std::regular_invocable<std::iter_value_t<Range>> Func>
auto strans(Range& input, Func&& func) {
    std::transform(std::ranges::begin(input), std::ranges::end(input),
            std::ranges::begin(input), std::forward<Func>(func));
    //return input;
}

template <std::ranges::input_range Range, std::regular_invocable<std::iter_value_t<Range>> Func>
auto pmrtrans(Range& input, Func&& func) {
    std::transform(std::execution::par_unseq, std::ranges::begin(input), std::ranges::end(input),
            std::ranges::begin(input), std::forward<Func>(func));
}

// ranges transform
template <std::ranges::input_range Range, std::regular_invocable<std::iter_value_t<Range>> Func>
auto rtrans(Range& input, Func&& func) {
    std::ranges::transform(std::ranges::begin(input), std::ranges::end(input),
            std::ranges::begin(input), std::forward<Func>(func));
    //return input;
}

// 自前分割統治スレッド version
template <std::ranges::input_range Range, std::regular_invocable<std::iter_value_t<Range>> Func>
auto thrtrans(Range& input, Func&& func) {
    std::vector<std::thread> threads;
    int thread_count = std::thread::hardware_concurrency();
    auto last = std::ranges::begin(input);
    auto size = input.size() / thread_count;
    for (int i = 0; i < thread_count; ++i) {
        auto first = std::move(last);
        last = (i == thread_count - 1) ? std::ranges::end(input) : first + size;

        threads.emplace_back([first, last, &func]() {
                std::transform(first, last, first, std::forward<Func>(func));
                });
    }
    for (int i = 0; i < thread_count; ++i) {
        threads[i].join();
    }
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

    auto fn = [](int const elem) { return elem * elem; };
    int thread_count = std::thread::hardware_concurrency();
    std::cout << "Hardware concurrency: " << thread_count << std::endl;

    auto r_strans = cpc::perf_timer<std::chrono::milliseconds>::duration([&data,&fn]() { strans(data, fn); });
    std::cout << "strans: " << r_strans.count() << "ms" << std::endl;
    //rtrans(data, fn);
    auto r_pmrtrans = cpc::perf_timer<std::chrono::milliseconds>::duration([&data,&fn]() { pmrtrans(data, fn); });
    std::cout << "pmrtrans: " << r_pmrtrans.count() << "ms" << std::endl;
    //rtrans(data, fn);
    auto r_rtrans = cpc::perf_timer<std::chrono::milliseconds>::duration([&data,&fn]() { rtrans(data, fn); });
    std::cout << "rtrans: " << r_rtrans.count() << "ms" << std::endl;
    //rtrans(data, fn);
    auto r_thrtrans = cpc::perf_timer<std::chrono::milliseconds>::duration([&data,&fn]() { thrtrans(data, fn); });
    std::cout << "thrtrans: " << r_thrtrans.count() << "ms" << std::endl;
}

int main(int, char** )
{
    test0();

    return 0;
}

/*
 * sample output
 * Hardware concurrency: 8
 * strans: 12ms
 * pmrtrans: 7ms
 * rtrans: 37ms
 * thrtrans: 2ms
 *
 * thread 自前で生成して vector を分割して並行処理するのが一番速かった。
 * ranges::transform は遅かったな・・。
 */
