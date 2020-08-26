#pragma once

namespace challenge100 {
template <class T>
constexpr T gcd(T a, T b) {
    return (b == 0) ? a : gcd(b, a % b);
}

}

