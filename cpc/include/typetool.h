#pragma once

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

    template <auto F>
        std::true_type detect_constexpr_invocable(decltype(int{(F(), 0u)}) = 0);

}
