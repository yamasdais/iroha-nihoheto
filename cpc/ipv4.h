#pragma once

#include <iostream>
#include <sstream>
#include <array>

namespace challenge100 {

struct ipv4 {
public:
    using nodeT = uint8_t;
private:
    std::array<nodeT, 4> data;

public:
    constexpr ipv4() : ipv4(0, 0, 0, 0) {}
    constexpr ipv4(nodeT a, nodeT b, nodeT c, nodeT d)
        : data{{a, b, c, d}} {}
    constexpr explicit ipv4(uint32_t a)
        : ipv4(static_cast<nodeT>((a >> 24) & 0xff),
               static_cast<nodeT>((a >> 16) & 0xff),
               static_cast<nodeT>((a >> 8) & 0xff),
               static_cast<nodeT>(a & 0xff)) {}
    ipv4(ipv4 const& other) noexcept
        : data(other.data) {}
    ipv4& operator=(ipv4 const& other) noexcept {
        data = other.data;
        return *this;
    }

    std::string to_string() const {
        std::stringstream sstr;
        sstr << *this;
        return sstr.str();
    }

    friend std::ostream& operator<<(std::ostream& os, ipv4 const& a) {
        os << static_cast<int>(a.data[0]) << '.'
           << static_cast<int>(a.data[1]) << '.'
           << static_cast<int>(a.data[2]) << '.'
           << static_cast<int>(a.data[3]);
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
};

}

