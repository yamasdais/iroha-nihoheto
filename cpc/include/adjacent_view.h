#pragma once

#include <iterator>
#include <ranges>
#include <concepts>
#include <utility>

#include "typetool.h"

namespace challenge100 {

template <std::ranges::input_range V, std::copy_constructible Func>
requires std::ranges::view<V> && std::is_object_v<Func>
    && std::regular_invocable<Func&, std::ranges::range_reference_t<V>, std::ranges::range_reference_t<V>>
class adjacent_view: public std::ranges::view_interface<adjacent_view<V, Func>> {
    template <bool Const> struct Sentinel;

    template <bool Const>
    struct Iterator {
      private:
        using Base = detail::maybe_const<Const, V>;
        using Parent = detail::maybe_const<Const, adjacent_view>;

        inline static constexpr
        auto Iter_Concept() {
            if constexpr (std::ranges::random_access_range<V>)
                return std::random_access_iterator_tag{};
            else if constexpr (std::ranges::bidirectional_range<V>)
                return std::bidirectional_iterator_tag{};
            else if constexpr (std::ranges::forward_range<V>)
                return std::forward_iterator_tag{};
            else 
                return std::input_iterator_tag{};
        }
        inline static constexpr
        auto Iter_Cat() {
            using ResT = std::invoke_result_t<Func&, std::ranges::range_reference_t<Base>,
                  std::ranges::range_reference_t<Base>>;
            if constexpr (std::is_lvalue_reference_v<ResT>) {
                using Cat = typename std::iterator_traits<BaseIter>::iterator_category;
                if constexpr (std::derived_from<Cat, std::contiguous_iterator_tag>)
                    return std::random_access_iterator_tag{};
                else
                    return Cat{};
            } else
                return std::input_iterator_tag{};
        }
        using BaseIter = std::ranges::iterator_t<Base>;

        BaseIter prev_Iter;
        BaseIter cur_Iter;
        Parent* parent_;

        constexpr static inline
        BaseIter takeSecondIter(Parent const& p, BaseIter const& itr) {
            auto end{std::ranges::end(p.base_)};
            if (itr == end) return end;
            auto next{std::next(itr)};
            return next == end ? end : next;
        }
        constexpr void
        advance() {
            prev_Iter = std::move(cur_Iter);
            cur_Iter = takeSecondIter(*parent_, prev_Iter);
        }

      public:
        using iterator_concept = decltype(Iter_Concept());
        using iterator_category = decltype(Iter_Cat());
        using value_type = std::remove_cvref_t<std::invoke_result_t<Func&,
              std::ranges::range_reference_t<Base>, std::ranges::range_reference_t<Base>>>;
        using difference_type = std::ranges::range_difference_t<Base>;

        constexpr
        Iterator() = default;
        constexpr
        Iterator(Parent& parent, BaseIter cur)
            : prev_Iter{std::move(cur)}
            , cur_Iter{takeSecondIter(parent, cur)}
            , parent_{std::addressof(parent)}
        {}

        constexpr decltype(auto)
        operator*() const
        noexcept(noexcept(std::invoke(parent_->func_, *prev_Iter, *cur_Iter)))
        {
            return std::invoke(parent_->func_, *prev_Iter, *cur_Iter);
        }
// とりあえず forward_iterator の機能だけ実装する
        constexpr Iterator&
        operator++()
        {
            advance();
            return *this;
        }
        constexpr void
        operator++(int){
            advance();
        }
        constexpr Iterator
        operator++(int) 
            requires std::ranges::forward_range<Base>
        {
            auto ret = *this;
            advance();
            return ret;
        }
        friend constexpr bool
        operator==(Iterator const& l, Iterator const& r)
        requires std::equality_comparable<BaseIter>
        {
            return l.cur_Iter == r.cur_Iter;
        }


        friend class Iterator<!Const>;
        template <bool>
        friend class Sentinel;
    };
    template <bool Const>
    struct Sentinel {
      private:
        using Base = detail::maybe_const<Const, V>;
        using Parent = detail::maybe_const<Const, adjacent_view>;

        std::ranges::sentinel_t<Base> end_ = std::ranges::sentinel_t<Base>{};

        template <bool LConst>
        constexpr std::ranges::range_difference_t<Base>
        distance_from(Iterator<LConst> const& itr) const {
            return end_ - itr.cur_Iter;
        }

        template <bool LConst>
        constexpr bool
        equal_to(Iterator<LConst> const& itr) const {
            return itr.cur_Iter == end_;
        }
      public:
        constexpr Sentinel() = default;
        constexpr
        Sentinel(std::ranges::sentinel_t<Base> end)
        : end_{end}
        {}

        constexpr std::ranges::sentinel_t<Base>
        base() const {
            return end_;
        }

        template <bool LConst>
            requires std::sentinel_for<std::ranges::sentinel_t<Base>,
                     std::ranges::iterator_t<detail::maybe_const<LConst, V>>>
        friend constexpr bool operator==(Iterator<LConst> const& itr, Sentinel const& en) {
            return en.equal_to(itr);
        }

        friend class Sentinel<!Const>;
    };
  public:
    constexpr
    adjacent_view() = default;
    constexpr
    adjacent_view(V base, Func func)
        : base_{std::move(base)}, func_{std::move(func)}
    {}


    constexpr V base() const&
        requires std::copy_constructible<V>
    { return base_; }
    constexpr V base() &&
    { return std::move(base_); }

    constexpr Iterator<false>
    begin()
    { return Iterator<false>{*this, std::ranges::begin(base_)}; }

    constexpr Iterator<true>
    begin() const
    { return Iterator<true>{*this, std::ranges::begin(base_)}; }

    constexpr Sentinel<false>
    end()
    { return Sentinel<false>{std::ranges::end(base_)}; }

    constexpr Iterator<false>
    end() requires std::ranges::common_range<V>
    { return Iterator<false>{*this, std::ranges::end(base_)}; }

    constexpr Sentinel<true>
    end() const
    { return Sentinel<true>{std::ranges::end(base_)}; }

    constexpr Iterator<true>
    end() const requires std::ranges::common_range<V>
    { return Iterator<true>{*this, std::ranges::end(base_)}; }

    constexpr
    auto size() requires std::ranges::sized_range<V>
    { return calc_size(); }
    constexpr
    auto size() const requires std::ranges::sized_range<V>
    { return calc_size(); }

  private:
    V base_;
    Func func_;

    constexpr auto calc_size() const
    requires std::ranges::sized_range<V>
    {
        auto base_size = std::ranges::size(base_);
        return base_size == 0u ? 0u : base_size - 1u;
    }
};

template <class Range, class Func>
adjacent_view(Range&&, Func) -> adjacent_view<std::views::all_t<Range>, Func>;

namespace detail {
    template <class Func>
    struct adjacent_range_adaptor_closure {
        Func func;
        constexpr adjacent_range_adaptor_closure(Func func)
            : func{std::move(func)}
        { }
        template <std::ranges::viewable_range Range>
        constexpr auto operator()(Range&& range) const {
            return adjacent_view(std::forward<Range>(range), func);
        }
    };

    struct adjacent_range_adaptor {
        template <std::ranges::viewable_range Range, class Func>
        constexpr auto operator()(Range&& range, Func&& func) const {
            return adjacent_view{std::forward<Range>(range), std::forward<Func>(func)};
        }
        template <class Func>
        constexpr auto operator()(Func&& func) const {
            return adjacent_range_adaptor_closure{std::forward<Func>(func)};
        }
    };

    template <std::ranges::viewable_range Range, class Func>
    constexpr auto operator|(Range&& range, adjacent_range_adaptor_closure<Func> const& clos) {
        return clos(std::forward<Range>(range));
    }
} // detail

namespace views {
    constexpr inline static detail::adjacent_range_adaptor adjacent;
}

}

namespace std::ranges {
    template <class Range, class Func>
        inline constexpr bool enable_borrowed_range<challenge100::adjacent_view<Range, Func>> = true;
}

