#include <concepts>
#include <iterator>
#include <ranges>

namespace challenge100 {

template <class AccFunc, std::weakly_incrementable Tval,
          std::semiregular Tsentinel = std::unreachable_sentinel_t>
class sequence_view
    : public std::ranges::view_interface<sequence_view<AccFunc, Tval, Tsentinel>> {
    AccFunc func;
    Tval init;
    Tsentinel bound;

   public:
    struct sentinel;

    struct iterator {
        using iterator_category = std::input_iterator_tag;
        using value_type = Tval;
        using difference_type = std::make_signed_t<size_t>;
        AccFunc func;
        Tval val;
        size_t index;

        iterator() = default;
        constexpr explicit iterator(AccFunc func, Tval val = {})
            : func(func), val(val), index(0) {}

        constexpr Tval operator*() const
            noexcept(std::is_nothrow_copy_constructible_v<Tval>) {
            return val;
        }

        constexpr iterator& operator++() {
            _advance();
            return *this;
        }

        constexpr void operator++(int) { _advance(); }

        constexpr iterator operator++(int) requires std::incrementable<Tval> {
            auto ret = (*this);
            _advance();
            return ret;
        }

        friend constexpr bool operator==(iterator const& x, iterator const& y) {
            return x.index == y.index;
        }

        friend constexpr difference_type operator-(iterator const& x, iterator const& y) {
            return static_cast<difference_type>(x.index) - y.index;
        }

       private:
        constexpr void _advance() {
            if constexpr (std::invocable<AccFunc, Tval, size_t>) {
                val = std::invoke(func, val, index);
            } else if constexpr (std::invocable<AccFunc, Tval>) {
                val = std::invoke(func, val);
            }
            index++;
        }

        constexpr iterator(size_t b) : index(b) {}
        friend sentinel;
        friend sequence_view<AccFunc, Tval, Tsentinel>;
    };

    class sentinel {
        constexpr bool equals(iterator const& x) { return x.index == bound; }
        Tsentinel bound;

       public:
        sentinel() : bound() {}

        constexpr explicit sentinel(Tsentinel b) : bound(b) {}

        friend constexpr bool operator==(iterator const& x, sentinel const& y) {
            return y.equals(x);
        }
    };

    constexpr sequence_view(AccFunc func, Tval init)
        : func(func), init(init), bound() {}

    constexpr sequence_view(AccFunc func, Tval init, size_t count)
        : func(func), init(init), bound(count) {}

    constexpr iterator begin() const {
        return iterator(func, init);
    }

    constexpr auto end() const {
        if constexpr (std::same_as<Tsentinel, std::unreachable_sentinel_t>) {
            return std::unreachable_sentinel;
        } else {
            return Tsentinel{bound};
        }
    }
    constexpr iterator end() const requires std::same_as<Tsentinel, size_t> {
        return iterator{bound};
    }
};

// deduction guide for sequence_view
template <class AccFunc, std::weakly_incrementable Tval>
sequence_view(AccFunc, Tval, size_t)
    -> sequence_view<AccFunc, Tval, size_t>;

}  // namespace challenge100
