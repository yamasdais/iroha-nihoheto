#pragma once
#include <vector>
#include <ranges>
#include <concepts>

#include "typetool.h"

namespace challenge100 {
#if 0
    template <class T>
    constexpr auto tt() {
        return T{}.empty();
    }
    template <class T>
        struct is_empty_constexpr_impl {
            static inline constexpr bool is_constexpr = T{}.empty();
        };

    template <class T, class = std::void_t<>>
    struct is_container_empty_constexpr : std::false_type {
    };
    template <class T>
    //struct is_container_empty_constexpr<T, decltype(tt<T>(nullptr))> : std::true_type {
    struct is_container_empty_constexpr<T, std::void_t<decltype(!is_empty_constexpr_impl<T>::is_constexpr)>> : std::true_type {
    };

    template <class, template <class> class, class = std::void_t<>>
        struct detect : std::false_type {};

    template <class T, template <class> class Checker>
        struct detect<T, Checker, std::void_t<Checker<T>>> : std::true_type {};
#endif

#if 0
    template <class T>
        bool constexpr is_empty_constexpr_impl(decltype(bool{T{}.empty()})) { return true; }
    template <class T>
        bool constexpr is_empty_constexpr_impl(...) { return false; }
    template <class T>
        using check_empty_constexpr = decltype(is_empty_constexpr_impl<T>(true));

    template <class T>
        using is_empty_constexpr = detect<T, check_empty_constexpr>;
#endif
#if 0
    template <class T, bool (T::*mf)() const noexcept>
        //std::true_type is_member_constexpr_concl(decltype(int{((T{}.*mf)(),0u)}));
        std::true_type is_member_constexpr_concl(decltype(int{((std::declval<T>().*mf)(),0u)}));
    template <class T, bool (T::*f)() const noexcept> std::true_type
        is_member_constexpr_impl(decltype(int{((T{}.*f)(),0u)}));
    template <class T, bool (T::*f)() const noexcept> std::false_type is_member_constexpr_impl(...);

    template <class T> using is_empty_constexpr1 = decltype(is_member_constexpr_impl<T, &T::empty>(0));
#endif

    template <class T>
        using check_m_empty_constexpr = decltype(
                detect_constexpr_invocable<[]() { return T{}.empty(); }>());
    template <class T>
        //using is_empty_constexpr = detect<T, check_m_empty_constexpr>;
        using is_empty_constexpr = is_detected<check_m_empty_constexpr, T>;

template <class T,
          std::ranges::random_access_range Container = std::vector<T>>
    requires std::ranges::sized_range<Container>
struct ring_buffer {
    using size_type = Container::size_type;
    using container_type = Container;
    //using foo = is_container_empty_constexpr<Container>;

    template <class... Args>
        requires std::constructible_from<Container, Args...>
    explicit ring_buffer(Args... arg)
    : c{std::forward<Args>(arg)...}
    {
    }

    void clear() noexcept {
    }

  protected:
    constexpr inline static size_type empty_head = static_cast<size_type>(-1);
    constexpr inline static size_type empty_size = static_cast<size_type>(0);
    Container c;
    size_type head_ = empty_head;
    size_type size_ = empty_size;
};

             
}
