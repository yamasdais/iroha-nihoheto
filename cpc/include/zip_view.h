#pragma once

#include <iterator>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <utility>

#include "challenge.h"

namespace challenge100 {

template <std::ranges::input_range V0, std::ranges::input_range V1, std::copy_constructible Func>
    requires std::ranges::view<V0> && std::ranges::view<V1> && std::is_object_v<Func>
    && std::regular_invocable<Func&, std::ranges::range_reference_t<V0>, std::ranges::range_reference_t<V1>> 
class zip_view : public std::ranges::view_interface<zip_view<V0, V1, Func>> {
    struct Sentinel;
    class Iterator {
        using Base0 = V0;
        using Base1 = V1;

        inline static constexpr
        auto IterConcept() {
            if constexpr (std::ranges::random_access_range<V0> && std::ranges::random_access_range<V1>)
                return std::random_access_iterator_tag{};
            else if constexpr (std::ranges::bidirectional_range<V0> && std::ranges::bidirectional_range<V1>)
                return std::bidirectional_iterator_tag{};
            else if constexpr (std::ranges::forward_range<V0> && std::ranges::forward_range<V1>)
                return std::forward_iterator_tag{};
            else
                return std::input_iterator_tag{};
        }

        using Parent = zip_view;
        using Base0Iter = std::ranges::iterator_t<Base0>;
        using Base1Iter = std::ranges::iterator_t<Base1>;

        Base0Iter base0Itr_;
        Base1Iter base1Itr_;
        Parent* parent_;

        constexpr void next() {
            ++base0Itr_;
            ++base1Itr_;
        }
      public:
        using iterator_concept = decltype(IterConcept());
        using iterator_category = decltype(IterConcept());
        using value_type = std::remove_cvref_t<std::invoke_result_t<Func&,
              std::ranges::range_reference_t<Base0>, std::ranges::range_reference_t<Base1>>>;
        using difference_type = std::ptrdiff_t;

        constexpr Iterator() = default;
        Iterator(Parent& parent, Base0Iter const& base0, Base1Iter const& base1)
            : base0Itr_{std::move(base0)}
            , base1Itr_{std::move(base1)}
            , parent_{std::addressof(parent)}
        {
        }

        constexpr decltype(auto)
        operator*() const
            noexcept(noexcept(std::invoke(parent_->func_, *base0Itr_, *base1Itr_)))
        {
            return std::invoke(parent_->func_, *base0Itr_, *base1Itr_);
        }

        constexpr
        Iterator& operator++()
        {
            next();
            return *this;
        }
        constexpr
        void operator++(int)
        {
            next();
        }
        constexpr
        Iterator& operator++(int)
            requires std::ranges::forward_range<Base0> && std::ranges::forward_range<Base1>
        {
            auto ret = *this;
            next();
            return ret;
        }

        friend constexpr
        bool operator==(Iterator const& l, Iterator const& r)
            requires std::equality_comparable<Base0Iter> && std::equality_comparable<Base1Iter>
        {
            return l.base0Itr_ == r.base0Itr_ && l.base1Itr_ == r.base1Itr_;
        }

        friend class Sentinel;
    };

    class Sentinel {
        using Parent = zip_view;
        using Base0 = V0;
        using Base1 = V1;

        std::ranges::sentinel_t<Base0> end0_ = std::ranges::sentinel_t<Base0>{};
        std::ranges::sentinel_t<Base1> end1_ = std::ranges::sentinel_t<Base1>{};

        constexpr
        bool or_equal_to(Iterator const& itr) const {
            return itr.base0Itr_ == end0_ || itr.base1Itr_ == end1_;
        }
      public:
        constexpr Sentinel() = default;
        Sentinel(std::ranges::sentinel_t<Base0> end0, std::ranges::sentinel_t<Base1> end1)
            : end0_{end0}, end1_{end1}
        { }

        constexpr std::ranges::sentinel_t<Base0>
        base() const {
            return end0_;
        }

        friend constexpr bool operator==(Iterator const& itr, Sentinel const& en) {
            return en.or_equal_to(itr);
        }

        friend class Iterator;
    };

    constexpr auto calc_size() const {
        return std::min(std::ranges::size(base0_), std::ranges::size(base1_));
    }

  public:

    constexpr
    zip_view() = default;

    constexpr
    zip_view(V0 base0, V1 base1, Func func)
        : base0_{std::move(base0)}, base1_{std::move(base1)}
        , func_{std::move(func)}
    {
    }

    constexpr V0 base() const&
    requires std::copy_constructible<V0>
    { return base0_; }
    constexpr V0 base() &&
    { return std::move(base0_); }

    constexpr
    Iterator begin() {
        return Iterator{*this, std::ranges::begin(base0_), std::ranges::begin(base1_)};
    }
    constexpr Iterator begin() const {
        return Iterator{*this, std::ranges::begin(base0_), std::ranges::begin(base1_)};
    }

    constexpr Sentinel end() {
        return Sentinel{std::ranges::end(base0_), std::ranges::end(base1_)};
    }
    constexpr Sentinel end() const {
        return Sentinel{std::ranges::end(base0_), std::ranges::end(base1_)};
    }

    constexpr auto size() requires std::ranges::sized_range<V0> && std::ranges::sized_range<V1>
    { return calc_size(); }
    constexpr auto size() const requires std::ranges::sized_range<V0> && std::ranges::sized_range<V1>
    { return calc_size(); }

  private:
    V0 const base0_;
    V1 const base1_;
    Func func_;
};

template <class Range0, class Range1, class Func>
zip_view(Range0&&, Range1&&, Func) -> zip_view<std::views::all_t<Range0>, std::views::all_t<Range1>, Func>;

namespace detail {
    template <class Range1, class Func>
    struct zip_range_adaptor_closure {
        Range1 const* range_;
        Func func_;
        constexpr zip_range_adaptor_closure(Range1 const& range, Func func)
            : range_{std::addressof(range)}
            , func_{std::move(func)}
        { }

        template <std::ranges::viewable_range Range0>
        constexpr auto operator()(Range0&& range0) const {
            return zip_view{std::forward<Range0>(range0), *range_, func_};
        }
    };
    struct zip_range_adaptor {
        template <std::ranges::viewable_range Range0, std::ranges::viewable_range Range1, class Func>
        constexpr auto operator()(Range0&& r0, Range1&& r1, Func func) const {
            return zip_view{std::forward<Range0>(r0), std::forward<Range1>(r1), std::forward<Func>(func)};
        }
        template <std::ranges::viewable_range Range1, class Func>
        constexpr auto operator()(Range1&& r1, Func&& func) const {
            return zip_range_adaptor_closure{std::forward<Range1>(r1), std::forward<Func>(func)};
        }
    };

    template <class Range0, class Range1, class Func>
    constexpr auto operator|(Range0&& r0, zip_range_adaptor_closure<Range1, Func> const& clos) {
        return clos(std::forward<Range0>(r0));
    }
} // cpc::detail

namespace views {
    constexpr inline static detail::zip_range_adaptor zip;
}

} // cpc

namespace std::ranges {
    template <class Range0, class Range1, class Func>
    inline constexpr bool enable_borrowed_range<challenge100::zip_view<Range0, Range1, Func>> = true;
}
