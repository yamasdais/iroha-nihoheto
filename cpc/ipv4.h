#pragma once

#include <iostream>
#include <sstream>
#include <array>
#include <compare>

namespace challenge100 {

struct ipv4 {
public:
    using nodeT = uint8_t;
private:
    union {
        std::array<nodeT, 4> ary;
        uint32_t val;
    } data;

public:
    constexpr ipv4() : ipv4(0, 0, 0, 0) {}
    constexpr ipv4(nodeT a, nodeT b, nodeT c, nodeT d)
        : data{ .ary={{d, c, b, a}}} {}
    constexpr explicit ipv4(uint32_t a)
        : data{ .val = a } {}
    ipv4(ipv4 const& other) noexcept
        : data{ .ary=other.data.ary } {}
    ipv4& operator=(ipv4 const& other) noexcept {
        data.ary = other.data.ary;
        return *this;
    }

    std::string to_string() const {
        std::stringstream sstr;
        sstr << *this;
        return sstr.str();
    }

    constexpr nodeT to_ulong() const noexcept {
        return data.val;
    }

    friend std::ostream& operator<<(std::ostream& os, ipv4 const& a) {
        os << static_cast<int>(a.data.ary[3]) << '.'
           << static_cast<int>(a.data.ary[2]) << '.'
           << static_cast<int>(a.data.ary[1]) << '.'
           << static_cast<int>(a.data.ary[0]);

        return os;
    }

    friend std::istream& operator>>(std::istream& is, ipv4& a) {
        char d1, d2, d3;
        int b1, b2, b3, b4;
        is >> b1 >> d1 >> b2 >> d2 >> b3 >> d3 >> b4;
        if (d1 == '.' && d2 == '.' && d3 == '.')
            a = ipv4(b1, b2, b3, b4);
        else
            is.setstate(std::ios_base::failbit);
        return is;
    }

    ipv4& operator++() {
        data.val++;
        return *this;
    }
    ipv4 operator++(int) {
        auto ret = *this;
        ++(*this);
        return ret;
    }

    auto operator<=>(ipv4 const& other) const {
        return data.val <=> other.data.val;
    }
    auto operator==(ipv4 const& other) const {
        return data.val == other.data.val;
    }
};

}

