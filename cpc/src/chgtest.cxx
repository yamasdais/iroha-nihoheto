#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <vector>

#include "challenge.h"
#include "sequence_view.h"
#include "str_util.h"

template <class T, class U>
concept asig = requires(T lhs, U&& rhs) {
    { lhs = std::forward<U&&>(rhs) } -> std::same_as<T>;
};
template <class T>
bool check_trait() {
    return std::movable<T>;
}

template <class T>
bool check_ItrTrait() {
    //return std::common_reference_with<const std::remove_reference_t<T&>&, const std::remove_reference_t<T>&>;
    //return asig<T&, T>;
    return std::forward_iterator<T>;
}

void test0() {
    auto constexpr fn = [](auto n) { return n * 2; };
    auto constexpr seq = cpc::sequence_view(fn, 1, 3);
    //auto itr = seq.begin();
    auto itr = std::ranges::begin(seq);
    //auto end = seq.end();
    auto end = std::ranges::end(seq);
    using fnT = decltype(fn);
    using seqT = decltype(seq);
    using itrT = decltype(itr);
    std::cout << std::boolalpha;
    std::cout << "lambda trait: " << check_trait<fnT>() << std::endl;
    std::cout << "sequence trait: " << check_trait<seqT>() << std::endl;
    std::cout << "typeof fn: " << cpc::nameT<fnT>() << std::endl;
    std::cout << "typeof seq: " << cpc::nameT<seqT>() << std::endl;
    std::cout << "typeof itr: " << cpc::nameT<itrT>() << std::endl;
    std::cout << "Concept: " << check_ItrTrait<itrT>() << std::endl;
    auto diff = seq.size();
    // auto diff = end - itr;
    // bool is_view = std::default_initializable<std::remove_const<seqT>>;
    bool is_view = std::ranges::view<seqT>;

    std::cout << "sizeof fn: " << sizeof(fn) << std::endl;
    std::cout << "sizeof seq: " << diff << std::endl;
    std::cout << "first: " << *itr++ << std::endl;
    std::cout << "is_view: " << is_view << std::endl;
    std::cout << "second: " << *itr++ << std::endl;
    std::cout << "isNotEnd: " << (itr != end) << std::endl;
    ;
    auto val3 = *itr;
    std::cout << "val3: " << val3 << std::endl;
    std::cout << "third: " << *itr++ << std::endl;
    // std::cout << "isEnd: " << std::weakly_incrementable<decltype(itr)>;
    std::cout << "isEnd: " << (itr == end) << std::endl;

    std::cout << std::endl;

    for (auto n : seq | std::views::take(4)) {
        std::cout << n << ",";
    }
    std::cout << std::endl;

    std::vector<std::ranges::range_value_t<decltype(seq)>> vec{seq.begin(),
                                                               seq.end()};
    std::cout << "sizeof vec: " << vec.size() << std::endl;
}

void test1() {
    std::cout << "test1()" << std::endl;
    auto constexpr seq = cpc::sequence_view(
        [](auto const acc, auto const n) { return acc + n * 2; }, 0);
    std::ranges::for_each(seq | std::views::take(20),
                          [](auto num) { std::cout << num << ", "; });
    std::cout << std::endl;
}

template <class T>
concept mem_begin = requires(T& t) {
    { t.begin() } -> std::forward_iterator;
};

struct fibonacci_fn {
    long pre;
    long operator()(long const num) {
        auto v = pre;
        pre = num;
        return v + pre;
    }
};

void fibonacci() {
    std::cout << "fibonacci()" << std::endl;
    // キャプチャ有 lambda 式だと iterator が input_or_output_iterator
    // (weakly_incrementable) と判定されなくなり、ranges::begin(seq) が
    // コンパイルエラーになってしまう。関数オブジェクトを関数として渡せば
    // ranges::being の concept 条件を満たすので、operator| で range を
    // つなぐことができるようだ。
#if 0
    auto fn = [prenum = 0](auto const acc) mutable {
        auto v = prenum;
        prenum = acc;
        return acc + v;
    };
#else
    auto fn = fibonacci_fn{1};
#endif
    auto seq = cpc::sequence_view(fn, 1, 24);
    auto itr = std::ranges::begin(seq);
    //auto itr = seq.begin();
    //auto end = seq.end();
    //auto end = std::ranges::end(seq);
    using fnT = decltype(fn);
    using seqT = decltype(seq);
    using itrT = decltype(itr);
    std::cout << "lambda trait: " << check_trait<fnT>() << std::endl;
    std::cout << "sequence trait: " << check_trait<seqT>() << std::endl;
    std::cout << "typeof fn: " << cpc::nameT<fnT>() << std::endl;
    std::cout << "typeof seq: " << cpc::nameT<seqT>() << std::endl;
    std::cout << "typeof itr: " << cpc::nameT<itrT>() << std::endl;
    std::cout << "ItrConcept: " << check_ItrTrait<itrT>() << std::endl;
    for (auto n : seq | std::ranges::views::take(12)) {
        std::cout << n << ",";
    }
    std::cout << std::endl;
}

void test2() {
    using namespace std::string_literals;
    try {
        for (auto&& token : cpc::split(std::string{"Foo bar baz"}, " "s)) {
            std::cout << "In loop" << "\n";
            //std::cout << "typeof token: " << cpc::nameT<decltype(token)>() << "\n";
            std::cout << token << ",\n";
        }
        std::cout << std::endl;
    } catch (std::exception& e) {
        std::cout << "caught exception: " << e.what() << std::endl;
    }
}

int main(int, char**) {
    test0();
    test1();
    test2();
    fibonacci();
    return 0;
}
