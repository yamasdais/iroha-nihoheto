#include <iostream>
#include <utility>
#include <concepts>

#if __has_include(<coroutine>)
# include <coroutine>
namespace crns = std;
#elif __has_include(<experimental/coroutine>)
# include <experimental/coroutine>
namespace crns = std::experimental;
#else
# error "Cannot find coroutine header."
#endif

template <class T>
struct generator {
    struct promise_type {
        T value;

        auto get_return_object() {
            return generator{*this};
        }

        auto initial_suspend()
        noexcept {
            return std::suspend_always{};
        }

        auto final_suspend()
        noexcept {
            return std::suspend_always{};
        }

        void return_value(T&& val) {
            value = std::forward<T>(val);
        }

        auto yield_value(T&& value) {
            value = std::forward<T>(value);
            return std::suspend_always{};
        }

        void unhandled_exception() {
            std::terminate();
        }
    };

    bool move_next() {
        if (handle) {
            handle.result();
            return !handle.done();
        }
        return false;
    }

    int current_value() {
        return handle.promise().value;
    }

    generator(generator const&) = delete;
    generator(generator&& rhs)
      : handle{ std::exchange(rhs.handle, nullptr) }
    {
    }

    ~generator() {
        if (handle)
            handle.destroy();
    }
private:
    explicit generator(promise_type& promise)
      : handle{ std::coroutine_handle<promise_type>::from_promise(promise) }
    {}

    std::coroutine_handle<promise_type> handle;
};

generator<int> cof() {
    std::cout << "in coroutine" << std::endl;
    co_yield 2;
    co_yield 42;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char*[]) {
    return 0;
}
