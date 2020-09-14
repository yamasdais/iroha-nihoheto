#include <array>

namespace challenge100 {

template <class T, size_t R, size_t C>
class array2d {
    std::array<T, R * C> ary;
    public:
    using value_type = T;
    using iterator = typename decltype(ary)::iterator;
    using const_iterator = typename decltype(ary)::const_iterator;

    array2d() {}

    template <class... E>
    explicit array2d(E&&... e)
        : ary{{std::forward<E>(e)...}} {}

    array2d(array2d&& other) = default;

    constexpr T* data() noexcept { return ary.data(); }
    constexpr T const* data() const noexcept { return ary.data(); }

    constexpr T& at(size_t const r, size_t const c) {
        return ary.at(r*C + c);
    }

    constexpr T const& at(size_t const r, size_t const c) const {
        return ary.at(r*C + c);
    }

    constexpr T& operator()(size_t const r, size_t const c) {
        return ary[r*C + c];
    }

    constexpr T const& operator()(size_t const r, size_t const c) const {
        return ary[r*C + c];
    }

    constexpr bool empty() const noexcept {
        return R == 0 || C == 0;
    }

    constexpr size_t size(int const rank) const {
        if (rank == 1) return R;
        if (rank == 2) return C;
        throw std::out_of_range("rank is out of range");
    }

    void fill(T const& value) {
        std::fill(std::begin(ary), std::end(ary), value);
    }

    void swap(array2d& other) {
        ary.swap(other.ary);
    }

    const_iterator begin() const { return ary.cbegin(); }
    const_iterator end() const { return ary.cend(); }
    iterator begin() { return ary.begin(); }
    iterator end() { return ary.end(); }
};

}

