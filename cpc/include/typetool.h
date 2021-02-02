#pragma once
#include <functional>
#include <type_traits>

namespace challenge100 {

    namespace detail {
        template <class Default, class Void, template <class...> class Checker, class... Args>
            struct detector {
                using value_t = std::false_type;
                using type = Default;
            };
        template <class Default, template <class...> class Checker, class... Args>
            struct detector<Default, std::void_t<Checker<Args...>>, Checker, Args...>
            {
                using value_t = std::true_type;
                using type = Checker<Args...>;
            };
    }
    template <template <class...> class Checker, class... Args>
        using is_detected = typename detail::detector<void*, void, Checker, Args...>::value_t;
    template <template <class...> class Checker, class... Args>
        constexpr inline bool is_detected_v = is_detected<Checker, Args...>::value;

    template <template <class...> class Checker, class... Args>
        using detected_t = typename detail::detector<void*, void, Checker, Args...>::type;
    template <class Default, template <class...> class Checker, class... Args>
        using detected_or_t = typename detail::detector<Default, void, Checker, Args...>::type;

    template <auto func, auto... args>
        requires std::invocable<decltype(func), decltype(args)...>
        std::true_type detect_constexpr_invocable(decltype(int{(std::invoke(func, args...), 0u)}) = 0);

#if 0
    // this cannot detect constexpr
    template <class Fn, class... Args>
        requires std::invocable<Fn, Args...>
        std::true_type detect_constexpr_invocable0(Fn func, Args... args,
                decltype(int{(std::invoke(func, args...), 0u)} = 0));
#endif

    template <class RefT, class T = std::remove_reference_t<RefT>>
        using is_forward_constructible = std::conditional_t<std::is_rvalue_reference_v<RefT>,
              typename std::is_move_constructible<T>::type, typename std::is_copy_constructible<T>::type>;

    template <class RefT, class T = std::remove_reference_t<RefT>>
        constexpr inline bool is_forward_constructible_v = is_forward_constructible<RefT, T>::value;

    template <class RefT, class T = std::remove_reference_t<RefT>>
        using is_nothrow_forward_constructible = std::conditional_t<std::is_rvalue_reference_v<RefT>,
              typename std::is_nothrow_move_constructible<T>::type,
              typename std::is_nothrow_copy_constructible<T>::type>;

    template <class RefT, class T = std::remove_reference_t<RefT>>
        constexpr inline bool is_nothrow_forward_constructible_v = is_nothrow_forward_constructible<RefT, T>::value;
}
