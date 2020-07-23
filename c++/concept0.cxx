#include <iostream>
#include <boost/type_index.hpp>

struct CT0 {
    int n;

    explicit CT0(int n)
    : n(n){}
};

struct CT1 {
    float n;

    explicit CT1(float n)
    : n(n){}
};

enum class EnType : int {
    Foo,
    Bar,
    Baz,
};

bool eq(CT0 const& lhs, CT0 const& rhs) {
    return lhs.n == rhs.n;
}

bool eq(CT1 const& lhs, CT1 const& rhs) {
    return lhs.n == rhs.n;
}

template <class T, class U>
concept CanCallEq = requires(T&& lhs, U&& rhs) {
    eq(std::forward<T>(lhs), std::forward<U>(rhs));
};

template <class T, class U>
bool fn0(T&& lhs, U&& rhs) {
    std::cout << "<" << boost::typeindex::type_id_with_cvr<T>().pretty_name()
    << ", " << boost::typeindex::type_id_with_cvr<U>().pretty_name() << ">: ";
    if constexpr(CanCallEq<T, U>) {
        std::cout << "Can call eq" << std::endl;
        return eq(std::forward<T>(lhs), std::forward<U>(rhs));
    } else {
        std::cout << "Cannot call eq." << std::endl;
        return std::equal_to{}(std::forward<T>(lhs), std::forward<U>(rhs));
    }
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    bool result;
    std::cout << std::boolalpha;
    const int iv = 3;
    result = fn0(iv, 42);

    CT1 c1v{1.2f};
    result = fn0(c1v, CT1{2.3});

    float fv = 3.1f;
    result = fn0(fv, 22.4f);

    EnType ev = EnType::Bar;
    result = fn0(ev, EnType::Foo);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
