#pragma once

#include <algorithm>
#include <functional>
#include <ranges>
#include <iterator>
#include <concepts>
#include <vector>
#include <type_traits>

namespace challenge100 {

// C++20 で導入された ranges projection で比較関数とかに使える lambda を生成する
// make_projection() って名前のほうが良いか
template <class Fn, class Proj, class... Args>
constexpr auto projection_helper(Fn&& fn, Proj&& proj) noexcept {
    return [func=std::forward<Fn>(fn), projection=std::forward<Proj>(proj)]
        (Args ...args)
        noexcept(noexcept(
            std::invoke(std::forward<Fn>(fn), std::invoke(std::forward<Proj>(proj), std::forward<Args>(args))...))) 
        {
            return std::invoke(func, std::invoke(projection, std::forward<Args>(args))...);
        };
}

// std::priority_queue を C++20 風にしたらこんな感じかなという実装
// 改善のアイデア
//  Compare, Proj にコンセプト追加
template <class T,
         std::ranges::random_access_range Cont = std::vector<T>,
         class Compare = std::less<T>,
         class Proj = std::identity>
class prior_que {
    using value_type = T;
    using size_type = std::ranges::range_size_t<Cont>;
    using reference = std::ranges::range_reference_t<Cont>;
    using const_reference = typename Cont::const_reference;
    using value_compare = 
        decltype(projection_helper<Compare, Proj, T const&, T const&>(std::declval<Compare>(), std::declval<Proj>()));

    Cont data_;
    value_compare comparer_;

public:
    explicit prior_que(Compare&& comparer = {}, Proj&& proj = {})
        : data_{},
        comparer_{projection_helper<Compare, Proj, T const&, T const&>(
                std::forward<Compare>(comparer), std::forward<Proj>(proj))
        }
    {
    }

    template <std::forward_iterator St, std::sentinel_for<St> En>
    prior_que(St start, En end, Compare&& comparer = {}, Proj&& proj = {})
    : data_{start, end},
      comparer_{projection_helper<Compare, Proj, T const&, T const&>(
              std::forward<Compare>(comparer), std::forward<Proj>(proj))
      }
    {
        if (!data_.empty())
            std::make_heap(std::ranges::begin(data_), std::ranges::end(data_), comparer_);
    }

    template <std::ranges::forward_range Src>
    explicit prior_que(Src const& container, Compare&& comparer = {}, Proj&& proj = Proj{})
        : prior_que(std::ranges::begin(container), std::ranges::end(container),
                std::forward<Compare>(comparer), std::forward<Proj>(proj))
    {
    }

    bool empty() const noexcept(noexcept(data_.empty())) {
        return data_.empty();
    }

    size_type size() const noexcept(noexcept(data_.size())) {
        return data_.size();
    }

    const_reference top() const {
        return data_.front();
    }

    void push(value_type const& val) {
        data_.push_back(val);
        std::push_heap(std::ranges::begin(data_), std::ranges::end(data_), comparer_);
    }
    void push(value_type&& val) {
        data_.push_back(std::move(val));
        std::push_heap(std::ranges::begin(data_), std::ranges::end(data_), comparer_);
    }

    template <class... Args>
    void emplace(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
        std::push_heap(std::ranges::begin(data_), std::ranges::end(data_), comparer_);
    }

    void pop() {
        std::pop_heap(std::ranges::begin(data_), std::ranges::end(data_), comparer_);
        data_.pop_back();
    }

    void swap(prior_que& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(comparer_, other.comparer_);
    }

    std::vector<T> const& data() const noexcept {
        return data_;
    }

    auto const& comparer() const noexcept {
        return comparer_;
    }
};

// deduction guide for prior_que.
// ソースコンテナなしの場合、要素型をコンストラクタに渡せないので、推論ガイドを書けない？
// make_prior_que<T>(comp, proj) みたいな make 関数を作る？

template <std::forward_iterator St, std::sentinel_for<St> End>
explicit prior_que(St, End)
    -> prior_que<std::iter_value_t<St>, std::vector<std::iter_value_t<St>>,
    std::less<std::iter_value_t<St>>, std::identity>;

template <std::forward_iterator St, std::sentinel_for<St> End, class Fn>
explicit prior_que(St, End, Fn&&)
    -> prior_que<std::iter_value_t<St>, std::vector<std::iter_value_t<St>>,
    Fn, std::identity>;

template <std::forward_iterator St, std::sentinel_for<St> End, class Fn, class Proj>
explicit prior_que(St, End, Fn&&, Proj&&)
    -> prior_que<std::iter_value_t<St>, std::vector<std::iter_value_t<St>>,
    Fn, Proj>;

template <std::ranges::forward_range Src>
explicit prior_que(Src)
    -> prior_que<std::ranges::range_value_t<Src>, std::vector<std::ranges::range_value_t<Src>>,
    std::less<std::ranges::range_value_t<Src>>, std::identity>;

template <std::ranges::forward_range Src, class Fn>
explicit prior_que(Src, Fn&&)
    -> prior_que<std::ranges::range_value_t<Src>, std::vector<std::ranges::range_value_t<Src>>,
    Fn, std::identity>;

template <std::ranges::forward_range Src, class Fn, class Proj>
explicit prior_que(Src, Fn&&, Proj&&)
    -> prior_que<std::ranges::range_value_t<Src>, std::vector<std::ranges::range_value_t<Src>>,
    Fn, Proj>;

}
