#include <chrono>
#include <tuple>
#include <concepts>
#include <thread>

#include "challenge.h"

template <class Time = std::chrono::microseconds,
          class Clock = std::chrono::high_resolution_clock>
struct perf_timer {
    template <class F, class... Args>
        requires std::invocable<F, Args...>
    static auto duration(F&& func, Args&& ...args) {
        auto start = Clock::now();
        if constexpr (std::is_invocable_r_v<void, F, Args...>) {
            std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
            return std::chrono::duration_cast<Time>(Clock::now() - start);
        } else {
            return std::tuple<std::invoke_result_t<F, Args...>, Time>{
                std::invoke(std::forward<F>(func), std::forward<Args>(args)...),
                std::chrono::duration_cast<Time>(Clock::now() - start),
            };
        }
    }
};

void f() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
}

void g(int const, int const) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
}


int main(int, char**) {
    auto t1 = perf_timer<>::duration(f);
    auto t2 = perf_timer<std::chrono::milliseconds>::duration(g, 1, 2);
    
    auto total = std::chrono::duration<double, std::nano>(t1 + t2).count();
    std::cout << "Total: " << total << std::endl;
    return 0;
}
