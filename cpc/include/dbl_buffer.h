#pragma once

#include <vector>
#include <type_traits>
#include <mutex>
#include <concepts>

#include "challenge.h"
#include "typetool.h"

namespace challenge100 {

template <class T,
         class Container = std::vector<T>>
struct dbl_buffer {
    using value_type = T;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    using pointer = std::add_pointer_t<value_type>;
    using size_type = size_t;
  private:
    template <class C> using check_container_size_constexpr = decltype(
            detect_constexpr_invocable<&C::size, C{}>()
            );
    static inline constexpr bool is_size_constexpr = is_detected_v<check_container_size_constexpr, Container>;

  public:
    template <class... Args>
    dbl_buffer(Args&&... arg)
    : rdbuf(std::forward<Args>(arg)...)
    , wrbuf(std::forward<Args>(arg)...)
    {}

    dbl_buffer(dbl_buffer& other) = delete;
    dbl_buffer(dbl_buffer&& other) = delete;

    void write(T const* const ptr, size_type const size) {
        std::unique_lock<std::mutex> lock(mt);
        auto len = std::min(size, wrbuf.size());
        std::copy(ptr, ptr + len, std::ranges::begin(wrbuf));
        wrbuf.swap(rdbuf);
    }

    template <class Out>
        requires std::output_iterator<Out, T>
    void read(Out it) const {
        std::unique_lock<std::mutex> lock(mt);
        std::copy(std::ranges::cbegin(rdbuf), std::ranges::cend(rdbuf), it);
    }

    pointer data() const {
        std::unique_lock<std::mutex> lock(mt);
        return rdbuf.data();
    }

    reference operator[](size_type const pos) {
        std::unique_lock<std::mutex> lock(mt);
        return rdbuf[pos];
    }

    const_reference operator[](size_type const pos) const {
        std::unique_lock<std::mutex> lock(mt);
        return rdbuf[pos];
    }

    void swap(dbl_buffer& other) {
        std::swap(rdbuf, other.rdbuf);
        std::swap(wrbuf, other.wrbuf);
    }

    [[nodiscard]]
    size_type size() const {
        if constexpr (is_size_constexpr) {
            return Container{}.size();
        } else {
            return rdbuf.size();
        } 
    }

  private:
    Container rdbuf;
    Container wrbuf;
    mutable std::mutex mt;
};

}
