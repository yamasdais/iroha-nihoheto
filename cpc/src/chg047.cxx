#include <thread>
#include <array>
#include <numeric>
#include <chrono>

#include "challenge.h"
#include "dbl_buffer.h"

template <class T>
void print_buffer(cpc::dbl_buffer<T> const& buf) {
    buf.read(std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::endl;
}

void test0() {
    constexpr size_t buf_size = 10;
    cpc::dbl_buffer<int> buf(buf_size);

    std::thread t([&buf]() {
        for (int i = 1; i < 1000; i += 10) {
            std::array<int, buf_size> data;
            std::iota(std::ranges::begin(data), std::ranges::end(data), i);
            buf.write(data.data(), std::size(data));

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
        }
    });

    auto start = std::chrono::system_clock::now();
    do {
        print_buffer(buf);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(150ms);
    } while (std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now() - start).count() < 12);

    t.join();
}

void test1() {
    constexpr size_t buf_size = 10;
    cpc::dbl_buffer<int> a(buf_size);
    cpc::dbl_buffer<int> b(buf_size);
    a.swap(b);
}

int main(int, char**) {
    test0();
    return 0;
}
