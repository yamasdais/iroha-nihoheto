#pragma once
#include <concepts>
#include <functional>
#include <type_traits>

#include "challenge.h"

// 任意個数の最小値取得関数
// 本の説明だと述語関数を受け取るバージョンは別の関数名とせざるを得ないとあったが、
// concept を使えば同じ関数名として実装できた。関数オブジェクトだけども。
// ということは C++17 でも SFINAE を駆使すれば同じ関数で実装できるんではなかろうか？
// （やりたくはないけど）
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
        requires std::invocable<minimum_fn, T, std::invoke_result_t<minimum_fn, U...>>
    constexpr auto operator()(T&& lhs, U&&... tail) const
    noexcept(noexcept((*this)(lhs, (*this)(tail...))))
        -> std::common_type_t<T, decltype((*this)(tail...))> {
        return (*this)(lhs, (*this)(tail...));
    }
#endif
    template <class F, class T, class U>
    requires std::convertible_to<std::invoke_result_t<F, T, U>, bool>
    constexpr std::common_type_t<T, U>
    operator()(F func, T&& lhs, U&& rhs) const
    noexcept(noexcept(std::invoke(func, std::forward<T>(lhs), std::forward<U>(rhs))))
    {
        return std::invoke(func, std::forward<T>(lhs), std::forward<U>(rhs))
                   ? lhs
                   : rhs;
    }

#if 1
    template <class F, class T, class... U>
    requires std::invocable<
        F, T,
        std::invoke_result_t<minimum_fn, F, U...>> constexpr auto
    operator()(F func, T&& lhs, U&&... tail) const
    noexcept(noexcept(std::invoke((*this), func, std::forward<T>(lhs),
                    std::invoke((*this), func, std::forward<U>(tail)...)))) {
        return std::invoke((*this), func, std::forward<T>(lhs),
                           std::invoke((*this), func, std::forward<U>(tail)...));
    }
#endif
};

inline constexpr minimum_fn minimum = {};

}  // namespace challenge100
