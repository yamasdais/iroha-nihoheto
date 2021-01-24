#include <chrono>
#include <tuple>
#include <concepts>
#include <thread>

#include "challenge.h"

void f() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
}

void g(int const, int const) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
}


int main(int, char**) {
    auto t1 = cpc::perf_timer<>::duration(f);
    auto t2 = cpc::perf_timer<std::chrono::milliseconds>::duration(g, 1, 2);
    
    auto total = std::chrono::duration<double, std::nano>(t1 + t2).count();
    std::cout << "Total: " << total << std::endl;
    return 0;
}
