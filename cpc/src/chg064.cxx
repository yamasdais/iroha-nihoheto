#include <iostream>
#include <vector>
#include <ranges>
#include <concepts>
#include <chrono>
#include <random>

#include "challenge.h"
#include "quicksort.h"

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
    auto stvec = data;

    auto stres = cpc::perf_timer<std::chrono::milliseconds>::duration([&stvec]() {
            cpc::quicksort(stvec);
            });

    std::cout << "Single thread: " << stres.count() << "ms\n";

    auto mtres = cpc::perf_timer<std::chrono::milliseconds>::duration([&data]() {
            cpc::parquicksort(data);
            });
    std::cout << "Async: " << mtres.count() << "ms\n";
}

int main(int, char**) {
    test0();

    return 0;
}

// 無理やり std::ranges::partition を使う実装をしてみたら、シングルスレッドより遅くなった
// 時間が出来たら std::partition を使うように改良しよう
