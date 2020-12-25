#include <iterator>
#include <coroutine>

#include "exception_util.h"

namespace challenge100 {

template <class T,
         bool EnableException = false>
struct generator {
    struct promise_type {
        T current_value;

        [[no_unique_address]]
        exception_rethrow_enabler<EnableException> thrower;

        auto get_return_object() {
            return generator{*this};
        }

        auto initial_suspend() noexcept {
            return std::suspend_always{};
        }

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        auto yield_value(T val) {
            current_value = val;
            return std::suspend_always{};
        }

        void return_void() noexcept {}
        void unhandled_exception()
            noexcept(noexcept(thrower.assign(nullptr)))
        {
            auto ex = std::current_exception();
            if (ex) {
                thrower.assign(ex);
            }
        }

        void rethrow()
            noexcept(noexcept(thrower.rethrow()))
        {
            thrower.rethrow();
        }
    };

    using coro_handle = std::coroutine_handle<promise_type>;

    struct iterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T const&;
        using pointer = T const*;

        coro_handle coro_;

        iterator() = default;
        iterator(nullptr_t) : coro_(nullptr) {}
        iterator(coro_handle ch) : coro_(ch) {}

        iterator& operator++() {
            coro_.resume();
            if (coro_.done()) {
                std::exchange(coro_, {}).promise().rethrow();
            }
            return *this;
        }
        void operator++(int) {
            ++*this;
        }

        [[nodiscard]] bool operator==(iterator const& rv) const {
            return coro_ == rv.coro_;
        }
        [[nodiscard]] bool operator!=(iterator const& rv) const {
            return !(*this == rv);
        }
        [[nodiscard]] reference operator*() const {
            return coro_.promise().current_value;
        }
        [[nodiscard]] pointer operator->() const {
            return std::addressof(coro_.promise().current_value);
        }
    };

    [[nodiscard]] iterator begin() {
        if (coro_) {
            coro_.resume();
            if (coro_.done()) {
                coro_.promise().rethrow();
                return {nullptr};
            }
        }
        return {coro_};
    }
    [[nodiscard]] iterator end() {
        return {nullptr};
    }

    explicit generator(promise_type& prom)
        : coro_{coro_handle::from_promise(prom)}
    {}
    generator() = default;
    generator(generator const&) = delete;
    generator& operator=(generator const&) = delete;

    generator(generator&& g)
        : coro_{g.coro_}
    {
        g.coro_ = nullptr;
    }
    generator& operator=(generator&& g) {
        if (this != std::addressof(g)) {
            std::swap(coro_, g.coro_);
        }
        return *this;
    }

    ~generator() {
        if (coro_) {
            coro_.destroy();
        }
    }

    private:
    coro_handle coro_ = nullptr;
};

} // closing namespace challenge100
