#include <iostream>
#include <utility>
#include <coroutine>

struct cogen {
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    struct promise_type {
        int current_value;
        static auto get_return_object_on_allocation_failure() {
            return cogen{nullptr};
        }
        auto get_return_object() {
            return cogen{handle::from_promise(*this)};
        }
        auto initial_suspend() noexcept {
            return std::suspend_always{};
        }
        auto final_suspend() noexcept {
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::terminate();
        }

        void return_void() {}
        auto yield_value(int value) {
            current_value = value;
            return std::suspend_always{};
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
