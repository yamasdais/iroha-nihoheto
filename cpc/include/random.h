#pragma once

#include <array>
#include <ranges>
#include <random>

namespace challenge100 {

namespace detail {

struct mt19937seed {
    static inline std::seed_seq& instance() {
        static bool first = true;
        std::array<std::seed_seq::result_type, std::mt19937::state_size> data;
        if (first) {
            std::random_device rd;
            std::ranges::generate(data, ref(rd));
            first = false;
        }
        static std::seed_seq seq(std::ranges::begin(data), std::ranges::end(data));
        return seq;
    }
};

} // ns: detail

template <class Dist>
struct random_generator {
    using result_type = typename Dist::result_type;

    template <class... Args>
    random_generator(Args&& ...arg)
    : distrib(std::forward<Args>(arg)...), mt{detail::mt19937seed::instance()}
    { }

    result_type operator()() {
        return distrib(mt);
    }


  private:
    Dist distrib;
    std::mt19937 mt;
};

} // ns:challenge100
