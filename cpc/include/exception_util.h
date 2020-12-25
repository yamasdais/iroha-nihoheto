#include <exception>

namespace challenge100 {
namespace detail {
    struct rethrow_abort {
        rethrow_abort() noexcept = default;
        rethrow_abort(std::exception_ptr) {
            std::abort();
        }
        void assign(std::exception_ptr) {
            std::abort();
        }
        void rethrow() noexcept {};
    };

    struct rethrower {
        std::exception_ptr _ex_ptr;
        rethrower() noexcept = default;
        rethrower(std::exception_ptr p) noexcept
            : _ex_ptr{p}
        {}
        void assign(std::exception_ptr p) noexcept {
            _ex_ptr = p;
        }
        void rethrow() {
            if (_ex_ptr) {
                std::rethrow_exception(_ex_ptr);
            }
        }
    };
} // namespace detail

template <bool IsEnable>
struct exception_rethrow_enabler;

template <>
struct exception_rethrow_enabler<false> 
: public detail::rethrow_abort {
    exception_rethrow_enabler() = default;
    exception_rethrow_enabler(std::exception_ptr p)
        : detail::rethrow_abort{p}
    {}
};

template <>
struct exception_rethrow_enabler<true> 
: public detail::rethrower {};

} // namespace challenge100
