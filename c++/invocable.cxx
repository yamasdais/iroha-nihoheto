#include <iostream>
#include <type_traits>
#include <boost/type_index.hpp>

struct A {};

struct B {};

// no overload
int foo([[maybe_unused]] A const& a, [[maybe_unused]] B const& b) {
    return 42;
}

// overload
int ovlFoo(int, [[maybe_unused]] A const& a) {
    return 9;
}

float ovlFoo(int, [[maybe_unused]] B const& b) {
    return 2.7182;
}

template <class T>
auto ti() {
    return boost::typeindex::type_id_with_cvr<T>();
}

template <class T, class U>
void showTypes() {
    std::cout << "(" << ti<T>().pretty_name() << ", " << ti<U>().pretty_name() << ")" << std::endl;
}
template <class T, class U>
void test0() {
    if constexpr (std::is_invocable_v<decltype(foo), T, U>) {
        std::cout << "Can call foo: ";
    } else {
        std::cout << "Cannot call foo: ";
    }
    showTypes<T, U>();
}

// raw sfinae
template <class T, class U>
class CanCallOvlFooSFINAE {
    template <class A0, class A1>
    static auto check(A0* t, A1* u) -> decltype(ovlFoo(*t, *u), std::true_type{});

    template <class, class>
    static std::false_type check(...);
public:
    static constexpr bool value = decltype(check<T, U>(nullptr, nullptr))::value;
};

template <class T, class U>
void testRawSFINAE() {
    if constexpr (CanCallOvlFooSFINAE<T, U>::value) {
        std::cout << "Can call ovlFoo[SFINAE]: ";
    } else {
        std::cout << "Cannot call ovlFoo[SFINAE]: ";
    }
    showTypes<T, U>();

}

// void_t sfinae
template <class, class, class = void>
struct CanCallOvlFooVoidSfinae : std::false_type {
};

template <class T, class U>
struct CanCallOvlFooVoidSfinae<T, U, std::void_t<decltype(ovlFoo(std::declval<T>(), std::declval<U>()))>>
: std::true_type {
};
template <class T, class U>
void testVoidSFINAE() {
    if constexpr (CanCallOvlFooVoidSfinae<T, U>::value) {
        std::cout << "Can call ovlFoo[void_t]: ";
    } else {
        std::cout << "Cannot call ovlFoo[void_t]: ";
    }
    showTypes<T, U>();
}

// is_detected
namespace detail {
   template <class A, template <class...> class Op, class... Args>
   struct is_detect_impl : std::false_type {};
   template <template <class...> class Op, class... Args>
   struct is_detect_impl<std::void_t<Op<Args...>>, Op, Args...> : std::true_type {};
}
template <template <class...> class Op, class... Args>
using my_detected = detail::is_detect_impl<void, Op, Args...>;

template <class T, class U>
using CanCallOvlFooDetectOp = decltype(ovlFoo(std::declval<T>(), std::declval<U>()));

template <class T, class U>
using CanCallOvlFooDetect = my_detected<CanCallOvlFooDetectOp, T, U>;

template <class T, class U>
void testDetectedSFINAE() {
    if constexpr (CanCallOvlFooDetect<T, U>::value) {
        std::cout << "Can call ovlFoo[detected]: ";
    } else {
        std::cout << "Cannot call ovlFoo[detected]: ";
    }
    showTypes<T, U>();
}

// invocable
// function object
struct CanCallOvlFooFO {
    template <class T, class U>
    auto operator()(T&& tv, U&& uv) -> decltype(ovlFoo(std::forward<T>(tv), std::forward<U>(uv)));
};

template <class T, class U>
void testInvocableFO() {
    if constexpr (std::is_invocable_v<CanCallOvlFooFO, T, U>) {
        std::cout << "Can call foo[FunctionObject]: ";
    } else {
        std::cout << "Cannot call foo[FunctionObject]: ";
    }
    showTypes<T, U>();
}

// generic lambda
template <class T, class U>
void testInvocableLmbd() {
    auto constexpr lmbd = [](auto&& t, auto&& u)
        -> decltype(ovlFoo(std::forward<decltype(t)>(t), std::forward<decltype(u)>(u))) {};
    if constexpr (std::is_invocable_v<decltype(lmbd), T, U>) {
        std::cout << "Can call foo[invocable(lambda)]: ";
    } else {
        std::cout << "Cannot call foo[invocable(lambda)]: ";
    }
    showTypes<T, U>();
}

// general generic lambda macro
#define ReturnTypeSnippet(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__)
#if defined(__cpp_generic_lambdas) && __cpp_generic_lambdas >= 201707L
#define MakeEmptyLambdaFromFunction(FUNC) \
    []<class... _a>(_a&&... arg) ReturnTypeSnippet(FUNC(std::forward<_a>(arg)...)) {}
#else
#define MakeEmptyLambdaFromFunction(FUNC) \
    [](auto&&... arg) ReturnTypeSnippet(FUNC(std::forward<decltype(arg)>(arg)...)) {}
#endif
template <class T, class U>
void testInvocableLambdaMacro() {
    auto constexpr lmbd = MakeEmptyLambdaFromFunction(ovlFoo);
    if constexpr (std::is_invocable_v<decltype(lmbd), T, U>) {
        std::cout << "Can call foo[invocable(make_lambda)]: ";
    } else {
        std::cout << "Cannot call foo[invocable(make_lambda)]: ";
    }
    showTypes<T, U>();
}


int main(int, char**) {
    test0<A, B>();
    test0<A, A>();

    testRawSFINAE<int, A>();
    testRawSFINAE<A, B>();

    testRawSFINAE<int, A>();
    testRawSFINAE<A, B>();

    testVoidSFINAE<int&&, A>();
    testVoidSFINAE<A&&, B>();

    testDetectedSFINAE<int&&, A>();
    testDetectedSFINAE<A&&, B>();

    testInvocableFO<int&&, A>();
    testInvocableFO<A&&, B>();

    testInvocableLmbd<int&&, A>();
    testInvocableLmbd<A&&, B>();

    testInvocableLambdaMacro<int&&, A>();
    testInvocableLambdaMacro<int, B const&>();
    testInvocableLambdaMacro<A&&, B>();

    return 0;
}
