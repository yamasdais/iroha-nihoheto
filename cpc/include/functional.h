#pragma once

#include <utility>
#include <type_traits>
#include <concepts>

namespace challenge100 {

struct accum_fn {
    template <std::forward_iterator IStart, std::sentinel_for<IStart> IEnd,
              class BinaryFn, class Init, class Proj = std::identity>
    requires std::convertible_to<
        std::invoke_result_t<BinaryFn, Init, std::iter_value_t<IStart>>,
        Init> constexpr Init
    operator()(IStart st, IEnd en, Init&& init, BinaryFn&& func,
               Proj proj = {}) const {
        Init ret = std::forward<Init>(init);
        for (; st != en; ++st) {
            ret = std::invoke(std::forward<BinaryFn>(func), std::move(ret),
                              std::invoke(proj, *st));
        }
        return ret;
    }

    template <std::ranges::forward_range Range, class BinaryFn, class Init,
              class Proj = std::identity>
    constexpr Init operator()(Range&& range, Init&& init, BinaryFn&& func,
                              Proj proj = {}) const {
        return (*this)(std::ranges::begin(range), std::ranges::end(range),
                       std::forward<Init>(init), std::forward<BinaryFn>(func),
                       std::forward<Proj>(proj));
    }
};

inline constexpr accum_fn accum = accum_fn{};

// C++20 で導入された ranges projection で比較関数とかに使える lambda を生成する
#if 0
// 初期バージョン
// 引数の型と同じ template 引数の型を書かなければならないのが邪魔くさかった
template <class Fn, class Proj, class... Args>
constexpr auto projection_helper(Fn&& fn, Proj&& proj) noexcept {
    return [func=std::forward<Fn>(fn), projection=std::forward<Proj>(proj)]
        (Args... args)
        noexcept(noexcept(
            std::invoke(std::forward<Fn>(fn), std::invoke(std::forward<Proj>(proj), std::forward<Args>(args))...))) 
        {
            return std::invoke(func, std::invoke(projection, std::forward<Args>(args))...);
        };
}
#endif
// 関数 template で一部型推論ガイド的な技法
namespace detail {
    template <class Fn, class Proj, class... Args>
        requires (std::invocable<Proj, Args> && ...)
        && std::invocable<Fn, std::invoke_result_t<Proj, Args>...>
    struct make_projection_impl {
        constexpr make_projection_impl(Fn&& func, Proj&& proj) noexcept
            : func{std::forward<Fn>(func)}, proj{std::forward<Proj>(proj)}
        {}

        constexpr auto operator()(Args... arg)
        const noexcept(noexcept(std::is_nothrow_invocable_v<Fn, std::invoke_result_t<Proj, Args>...>))
        {
            return std::invoke(func, std::invoke(proj, std::forward<Args>(arg))...);
        }

        Fn func;
        Proj proj;
    };

}

template <class... Args>
constexpr auto make_projection = [](auto&& fn, auto&& proj) noexcept {
    return detail::make_projection_impl<
        std::remove_reference_t<decltype(fn)>, std::remove_reference_t<decltype(proj)>,
        Args...>{
            std::forward<std::remove_reference_t<decltype(fn)>>(fn),
            std::forward<std::remove_reference_t<decltype(proj)>>(proj)
        };
};
}
