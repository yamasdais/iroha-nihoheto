#include <iostream>
#include <utility>
#if defined(_MSC_VER)
# include <experimental/coroutine>
#else
# include <coroutine>
#endif

#if defined(_MSC_VER)
namespace crns = std::experimental;
#else
namespace crns = std;
#endif

struct cogen {
    struct promise_type;
    using handle = crns::coroutine_handle<promise_type>;
    struct promise_type {
        int current_value;
        static auto get_return_object_on_allocation_failure() {
            return cogen{nullptr};
        }
        auto get_return_object() {
            return cogen{handle::from_promise(*this)};
        }
        auto initial_suspend() noexcept {
            return crns::suspend_always{};
        }
        auto final_suspend() noexcept {
            return crns::suspend_always{};
        }
        void unhandled_exception() {
            std::terminate();
        }

        void return_void() {}
        auto yield_value(int value) {
            current_value = value;
            return crns::suspend_always{};
        }
    };

    bool move_next() {
        return coro
          ? (coro.resume(), !coro.done())
          : false;
    }

    int current_value() {
        return coro.promise().current_value;
    }

    cogen(cogen const&) = delete;
    cogen(cogen&& src)
      : coro(std::exchange(src.coro, nullptr)) {
    }

    ~cogen()
    {
        if (coro)
           coro.destroy();
    }

    cogen& operator=(cogen const&) = delete;

  private:
    handle coro;
    cogen(handle hnd)
    : coro(hnd) {
    }
};

// coroutine
cogen f() {
    std::cout << "Start f()" << std::endl;
    co_yield 1;
    std::cout << "resumed after yield 1" << std::endl;
    co_yield 2;
    std::cout << "resumed after yield 2" << std::endl;
}

void coro_loop() {
    auto coroutine_ver =
#if defined(__cpp_coroutines)
        __cpp_coroutines
#else
        0
#endif
        ;
    std::cout << "__cpp_coroutines: " << coroutine_ver << std::endl;
    auto g = f();
    std::cout << "before loop" << std::endl;
    while (g.move_next()) {
        std::cout << "before getvalue line" << std::endl;
        std::cout << "co value:" << g.current_value() << std::endl;
        std::cout << "after getvalue line" << std::endl;
    }
}

int main(int, char**) {
    coro_loop();
    return 0;
}
