#pragma once

#include <concepts>
#include <ranges>
#include <iterator>
#include <future>

namespace challenge100 {

struct partition_fn {
    template <std::permutable I, std::sentinel_for<I> S,
              class Proj = std::identity,
              std::indirect_unary_predicate<std::projected<I, Proj>> Pred>
    constexpr std::ranges::subrange<I>
    operator()(I first, S last, Pred pred, Proj proj = {}) const {
        first = std::ranges::find_if_not(first, last, std::ref(pred), std::ref(proj));
        if (first == last)
            return {first, first};
        auto begin = first;
        for (auto i = std::ranges::next(first); i != last; ++i) {
            if (std::invoke(pred, std::invoke(proj, *i))) {
                std::ranges::iter_swap(i, first);
                ++first;
            }
        }
        return {std::move(begin), std::move(first)};
    }

    template <std::ranges::forward_range Range, class Proj = std::identity,
              std::indirect_unary_predicate<std::projected<std::ranges::iterator_t<Range>, Proj>> Pred>
        requires std::permutable<std::ranges::iterator_t<Range>>
    constexpr std::ranges::borrowed_subrange_t<Range>
    operator()(Range&& r, Pred pred, Proj proj = {}) const {
        return operator()(std::ranges::begin(r), std::ranges::end(r), std::ref(pred), std::ref(proj));
    }
};

inline constexpr partition_fn partition{};

struct quicksort_fn {
    template <std::permutable I, std::sentinel_for<I> S, class Comp = std::ranges::less, class Proj = std::identity>
        requires std::sortable<I, Comp, Proj>
    constexpr void operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
    {
        if (first == last) {
            return;
        }
        auto pivot = *std::ranges::next(first, std::ranges::distance(first, last)/2, last);
        std::ranges::subrange tail1 = std::ranges::partition(first, last, [&pivot, &comp, &proj](auto const& e) {
                return std::invoke(comp, std::invoke(proj, e), std::invoke(proj, pivot));
                });
        std::ranges::subrange tail2 = std::ranges::partition(tail1,
                [&pivot, &comp, &proj](auto const& e) {
                    return !std::invoke(comp, std::invoke(proj, pivot), std::invoke(proj, e));
                });
        operator()(first, std::ranges::begin(tail1), comp, proj);
        operator()(std::ranges::begin(tail2), std::ranges::end(tail2), comp, proj);
    }
    template <std::ranges::forward_range Range, class Comp = std::ranges::less, class Proj = std::identity>
        requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
    constexpr void operator()(Range&& range, Comp&& comp = {}, Proj&& proj = {}) const
    {
        operator()(std::ranges::begin(range), std::ranges::end(range),
                std::forward<Comp>(comp), std::forward<Proj>(proj));
    }
};

inline constexpr quicksort_fn quicksort{};

struct parquicksort_fn {
    template <std::permutable I, std::sentinel_for<I> S, class Comp = std::ranges::less, class Proj = std::identity>
        requires std::sortable<I, Comp, Proj>
    constexpr void operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
    {
        if (first >= last)
            return;
        auto const size = std::distance(first, last);
        if (size <= 100'000) {
            quicksort(first, last, std::forward<Comp>(comp), std::forward<Proj>(proj));
        } else {
            auto pivot = *std::ranges::next(first, std::ranges::distance(first, last)/2, last);
            std::ranges::subrange tail1 = std::ranges::partition(first, last,
                    [&pivot, &comp, &proj](auto const& e) {
                        return std::invoke(comp, std::invoke(proj, e), std::invoke(proj, pivot));
                    });
            auto invpart = [&comp, &proj, &pivot](auto const& e) {
                return !std::invoke(comp, std::invoke(proj, pivot), std::invoke(proj, e));
            };
            auto f1 = std::async(std::launch::async, [&tail1, &invpart]() {
                    return std::ranges::partition(tail1, invpart);
                    });
            operator()(f1.get(), comp, proj);
            operator()(first, std::ranges::begin(tail1), comp, proj);
        }
    }
    template <std::ranges::forward_range Range, class Comp = std::ranges::less, class Proj = std::identity>
        requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
    constexpr void operator()(Range&& range, Comp&& comp = {}, Proj&& proj = {}) const
    {
        operator()(std::ranges::begin(range), std::ranges::end(range),
                std::forward<Comp>(comp), std::forward<Proj>(proj));
    }
};

inline constexpr parquicksort_fn parquicksort{};

}

