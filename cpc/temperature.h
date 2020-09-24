#pragma once
#include <compare>

#include "challenge.h"

namespace challenge100::temperature {

enum class scale {
    celsius,
    fahrenheit,
    kelvin,
};

template <scale S, scale R>
struct conversion_traits;

template <scale S>
class quantity {
    double const amount;

   public:
    constexpr explicit quantity(double const q) : amount(q) {}
    constexpr explicit operator double() const { return amount; }

    template <scale R>
    constexpr quantity<R> convert() const {
        return quantity<R>{
            conversion_traits<S, R>::convert(static_cast<double>(amount))};
    }

    template <scale Sv>
    friend std::ostream& operator<<(std::ostream&, quantity<Sv> const&);
};

inline std::ostream& operator<<(std::ostream& os, quantity<scale::celsius> const& q) {
    return os << static_cast<double>(q) << "℃";
}

inline std::ostream& operator<<(std::ostream& os, quantity<scale::fahrenheit> const& q) {
    return os << static_cast<double>(q) << "°F";
}

inline std::ostream& operator<<(std::ostream& os, quantity<scale::kelvin> const& q) {
    return os << static_cast<double>(q) << "K";
}

// operators
template <scale S>
constexpr inline bool operator==(quantity<S> const& lhs,
                                 quantity<S> const& rhs) {
    return are_equal(static_cast<double>(lhs), static_cast<double>(rhs));
}

template <scale S>
constexpr inline std::partial_ordering operator<=>(quantity<S> const& lhs,
                                                quantity<S> const& rhs) {
    if (are_equal(static_cast<double>(lhs), static_cast<double>(rhs)))
        return std::partial_ordering::equivalent;
    return static_cast<std::partial_ordering>(static_cast<double>(lhs) <=> static_cast<double>(rhs));
}

template <scale S>
constexpr inline quantity<S> operator+(quantity<S> const& lhs,
                                       quantity<S> const& rhs) {
    return quantity<S>{static_cast<double>(lhs) + static_cast<double>(rhs)};
}

template <scale S>
constexpr inline quantity<S> operator-(quantity<S> const& lhs,
                                       quantity<S> const& rhs) {
    return quantity<S>{static_cast<double>(lhs) - static_cast<double>(rhs)};
}

// conversion_traits implementation
template <>
struct conversion_traits<scale::celsius, scale::fahrenheit> {
    constexpr static double convert(double const value)
    {
        return (value * 9) / 5 + 32;
    }
};

template <>
struct conversion_traits<scale::fahrenheit, scale::celsius> {
    constexpr static double convert(double const value)
    {
        return (value - 32) * 5 / 9;
    }
};

template <>
struct conversion_traits<scale::celsius, scale::kelvin> {
    constexpr static double convert(double const value)
    {
        return value + 273.15;
    }
};

template <>
struct conversion_traits<scale::kelvin, scale::celsius> {
    constexpr static double convert(double const value)
    {
        return value - 273.15;
    }
};

template <>
struct conversion_traits<scale::kelvin, scale::fahrenheit> {
    constexpr static double convert(double const value)
    {
        return ((value - 273.15) * 9) / 5 + 32;
    }
};

template <>
struct conversion_traits<scale::fahrenheit, scale::kelvin> {
    constexpr static double convert(double const value)
    {
        return ((value - 32) * 5) / 9 + 273.15;
    }
};

// literal
namespace literal {
constexpr quantity<scale::celsius> operator""_deg(long double const amount) {
    return quantity<scale::celsius>{static_cast<double>(amount)};
}

constexpr quantity<scale::fahrenheit> operator""_f(long double const amount) {
    return quantity<scale::fahrenheit>{static_cast<double>(amount)};
}

constexpr quantity<scale::kelvin> operator""_k(long double const amount) {
    return quantity<scale::kelvin>{static_cast<double>(amount)};
}

}  // namespace literal

}  // namespace challenge100::temperature
