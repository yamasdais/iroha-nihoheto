#include <iostream>
#include <thread>
#include <mutex>
#include <ranges>
#include <random>
#include <string_view>

#include "challenge.h"

struct logger {
    static logger& instance() {
        static logger log__;
        return log__;
    }
    logger(logger const&) = delete;
    logger& operator=(logger const&) = delete;
    logger(logger&&) = delete;
    logger& operator=(logger&&) = delete;

    void log(std::string_view const& message) {
        std::lock_guard lock(mt);
        std::cout << "LOG: " << message << std::endl;
    }

  protected:
    logger() = default;

  private:
    std::mutex mt;
};

void test0() {
    std::vector<std::thread> modules;

    for (auto const id : std::views::iota(0, 5)) {
        modules.emplace_back([id]() {
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<> ud(100, 1000);
                auto ident = "module " + std::to_string(id);

                logger::instance().log(ident + " started");

                std::this_thread::sleep_for(std::chrono::milliseconds(ud(mt)));

                logger::instance().log(ident + " finished");
                });
    }

    for (auto& m : modules) {
        m.join();
    }
}

int main(int, char**) {
    test0();

    return 0;
}
