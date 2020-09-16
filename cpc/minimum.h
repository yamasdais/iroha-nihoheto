#pragma once
#include <concepts>
#include <functional>
#include <type_traits>

#include "challenge.h"

namespace challenge100 {
// minimum

struct minimum_fn {
    template <class T, class U>
    requires requires(T&& t, U&& u) {
        { t <= u }
        ->std::convertible_to<bool>;
    }
    constexpr std::common_type_t<T, U> operator()(T&& lhs, U&& rhs) const
        noexcept(noexcept(lhs <= rhs)) {
        return lhs <= rhs ? lhs : rhs;
    }

#if 1
    template <class T, class... U>
    constexpr auto operator()(T&& lhs, U&&... tail) const
        -> std::common_type_t<T, decltype((*this)(tail...))> {
        return (*this)(lhs, (*this)(tail...));
    }
#endif
    template <class F, class T, class U>
    requires std::invocable<F, T, U> constexpr std::common_type_t<T, U>
    operator()(F func, T&& lhs, U&& rhs) const {
        return std::invoke(func, std::forward<T>(lhs), std::forward<U>(rhs))
                   ? lhs
                   : rhs;
    }

#if 1
    template <class F, class T, class... U>
    requires std::invocable<
        F, T,
        std::invoke_result_t<minimum_fn, F, U...>> constexpr auto
    operator()(F func, T&& lhs, U&&... tail) const {
        return std::invoke((*this), func, std::forward<T>(lhs),
                           std::invoke((*this), func, std::forward<U>(tail)...));
    }
#endif
};

inline constexpr minimum_fn minimum = {};

}  // namespace challenge100
