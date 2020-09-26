#include <iomanip>
#include <sstream>

#include "challenge.h"

// binary sequence to string
struct bytes_to_hexstr_fn {
    template <std::forward_iterator IStart, std::sentinel_for<IStart> IEnd,
              class T = std::iter_value_t<IStart>>
    std::string operator()(IStart begin, IEnd end,
                           bool const uppercase = false) const {
        std::ostringstream oss;
        if (uppercase) oss << std::uppercase; // = oss.setf(std::ios_base::uppercase)
        for (; begin != end; ++begin) {
            oss << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(*begin);
        }
        return oss.str();
    }

    template <std::ranges::forward_range Range>
    std::string operator()(Range&& range, bool const uppercase = false) const {
        return (*this)(std::ranges::begin(range), std::ranges::end(range),
                       uppercase);
    }
};

inline constexpr bytes_to_hexstr_fn bytes_to_hexstr = {};

template <std::ranges::forward_range Range>
void print_range(Range&& range, char const* prefix, bool const uppercase = false) {
    std::cout << prefix << bytes_to_hexstr(std::forward<Range>(range), uppercase) << std::endl;
}

int main(int, char**) {
    std::vector<unsigned char> const v{0xba, 0xad, 0xf0, 0x0d};
    std::array<unsigned char, 6> const a {{ 0, 1, 2, 3, 4, 5 }};
    unsigned char buf[5] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
    //std::cout << "v: " << bytes_to_hexstr(v) << std::endl;
    print_range(v, "v: ", true);
    print_range(a, "a: ", true);
    print_range(buf, "buf: ", true);
    print_range(v, "v: ");
    print_range(a, "a: ");
    print_range(buf, "buf: ");
    return 0;
}
