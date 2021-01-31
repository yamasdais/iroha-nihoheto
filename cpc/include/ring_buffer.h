#pragma once
#include <cassert>
#include <vector>
#include <ranges>
#include <type_traits>
#include <concepts>

#include "typetool.h"

namespace challenge100 {

#if 0
    template <class T>
        using check_m_empty_constexpr = decltype(
                //detect_constexpr_invocable<[]() { return T{}.empty(); }>()
                detect_constexpr_invocable<&T::empty, T{}>()  // ←↑ どっちでも機能する
                );
    template <class T>
        //using is_empty_constexpr = detect<T, check_m_empty_constexpr>;
        using is_empty_constexpr = is_detected<check_m_empty_constexpr, T>;
#endif

template <class T,
          std::ranges::random_access_range Container = std::vector<T>>
    requires std::ranges::sized_range<Container>
struct ring_buffer {
    struct iterator;

    using size_type = Container::size_type;
    using container_type = Container;
    using const_iterator = std::add_const_t<iterator>;

  private:
    template <class C>
        using check_container_empty_constexpr = decltype(
                //detect_constexpr_invocable<[]() { return ContT{}.empty(); }>()
                detect_constexpr_invocable<&C::empty, C{}>()  // ←↑ どっちでも機能する
                );
    static inline constexpr bool is_empty_constexpr = is_detected_v<check_container_empty_constexpr, Container>;

    size_type next_pos() const noexcept {
        if constexpr (is_empty_constexpr)  {
            static_assert(!Container{}.empty(), "requires non-empty container.");
        }
        return size_ == 0
            ? 0
            : (head_ + 1) % c.size();
    }

    size_type first_pos () const noexcept {
        if constexpr (is_empty_constexpr)  {
            static_assert(!Container{}.empty(), "requires non-empty container.");
        }
        return size_ == 0
            ? 0
            : (head_ + c.size() - size_ + 1) % c.size();
    }

  public:
    template <class... Args>
        requires std::constructible_from<Container, Args...>
    explicit ring_buffer(Args... arg)
    noexcept(std::is_nothrow_constructible_v<Container, Args...> && is_empty_constexpr)
    : c{std::forward<Args>(arg)...}
    {
        if constexpr (!is_empty_constexpr)  {
            if (c.empty())
                throw std::range_error("the container must not be empty");
        }
    }
#if 0
    explicit ring_buffer(Container const& cont)
        noexcept(std::is_nothrow_copy_constructible_v<Container>)
        requires std::is_copy_constructible_v<Container>
        :c{cont}
    {
    }
#endif
    explicit ring_buffer(Container&& cont)
        noexcept(std::is_rvalue_reference_v<Container>
                ? std::is_nothrow_move_constructible_v<Container>
                : std::is_copy_constructible_v<Container>)
        requires (std::is_rvalue_reference_v<Container>
                ? std::is_move_constructible_v<Container>
                : std::is_copy_constructible_v<Container>)
        :c{std::forward<Container>(cont)}
    {}

    void clear() noexcept {
        head_ = empty_head;
        size_ = empty_size;
    }
    [[nodiscard]]
    bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]]
    bool full() const noexcept { return size_ == c.size(); }
    [[nodiscard]]
    size_t capacity() const noexcept { return c.size(); }
    [[nodiscard]]
    size_t size() const noexcept { return size_; }

    std::ostream& trace(std::ostream& out) {
        out << "[" << head_ << "," << size_ << "]:{";
        bool isFirst = true;
        for (auto& v : c) {
            if (!isFirst)
                out << ",";
            else
                isFirst = false;
            out << v;
        }
        out << "} -> {";
        isFirst = true;
        for (auto& v : *this) {
            if (!isFirst)
                out << ",";
            else
                isFirst = false;
            out << v;
        }

        out << "}\n";

        return out;
    }

    void push(T&& item)
        noexcept(noexcept(next_pos() && c.operator[](item)))
    {
        head_ = next_pos();
        c[head_] = std::forward<T>(item);
        if (size_ < c.size())
            size_++;
        std::cout << "push(" << item << ")";
        trace(std::cout);
    }

    T pop()
    {
        if (empty())
            throw std::runtime_error("cannot pop from empty buffer");

        auto pos = first_pos();
        size_--;
        std::cout << "pop()->" << c[pos];
        trace(std::cout);
        return c[pos];
    }
    const_iterator begin() const noexcept {
        return iterator{*this, first_pos(), empty()};
    }
    const_iterator end() const noexcept {
        return iterator{*this, next_pos(), true};
    }

    friend class iterator;
    struct sentinel;
    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using reference = std::add_lvalue_reference_t<T>;
        using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
        using pointer = std::add_pointer_t<T>;
        using const_pointer = std::add_const_t<pointer>;
        using difference_type = ptrdiff_t;

        iterator(ring_buffer const& buf, size_type const pos, bool const last)
            noexcept
            : buffer_{buf}, index_{pos}, last_{last}
        {}

        iterator& operator++() noexcept(false) {
            if (last_)
                throw std::out_of_range("iterator cannot be increment");
            index_ = (index_ + 1) % buffer_.c.size();
            last_ = index_ == buffer_.next_pos();
            return *this;
        }
        iterator operator++(int) {
            auto ret = *this;
            ++*this;
            return ret;
        }

        bool operator==(iterator const& other) const noexcept {
            assert(compatible(other));
            return index_ == other.index_ && last_ == other.last_;
        }
        bool operator!=(iterator const& other) const noexcept {
            return !operator==(other);
        }

        const_reference operator*() const noexcept(noexcept(buffer_.c[index_])) {
            return buffer_.c[index_];
        }
        const_pointer operator->() const noexcept(noexcept(operator*())) {
            return std::addressof(operator*());
        }

        private:
        bool compatible(iterator const& other) const noexcept {
            return std::addressof(buffer_) == std::addressof(other.buffer_);
        }
        ring_buffer const& buffer_;
        size_type index_;
        bool last_;
    };

    private:
  protected:
    constexpr inline static size_type empty_head = static_cast<size_type>(-1);
    constexpr inline static size_type empty_size = static_cast<size_type>(0);
    Container c;
    size_type head_ = empty_head;
    size_type size_ = empty_size;
};

             
}
