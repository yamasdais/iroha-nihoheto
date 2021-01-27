#include <list>

#include "challenge.h"
#include "prior_que.h"

struct foo {
    int num;
    std::string text;

    friend std::ostream& operator<<(std::ostream& out, foo const& obj) {
        out << "{" << obj.num << "," << obj.text << "}";
        return out;
    }
};

void test_projection() {
    std::list<foo> cont;

    cont.emplace_back(1, "foo");
    cont.emplace_back(4, "bar");
    auto less = cpc::projection_helper<std::less<int>, int foo::*, foo const&, foo const&>({}, &foo::num);
    //auto result = less(cont[0], cont[1]);
    //cpc::prior_que q{cont.begin(), cont.end(), std::greater<int>{}, &foo::num};
    //cpc::prior_que q{std::ranges::begin(cont), std::ranges::end(cont), std::greater<int>{}, &foo::num};
    cpc::prior_que q{cont, std::greater<int>{}, &foo::num};

    for (auto const& pq : q.data()) {
        std::cout << pq << std::endl;
    }

    auto result = less(q.data()[0], q.data()[1]);
    //cpc::TypeTracer<typename decltype(q)::value_compare> x;
    //cpc::TypeTracer<decltype(less)> x;

    std::cout << "Projection result: " << result << std::endl;

}

void test0() {
    std::vector<int> vec = { 3, 65, 2, 42, 7, 21 };
    //cpc::prior_que<int> q{std::ranges::begin(vec), std::ranges::end(vec)};
    //auto q = cpc::prior_que{std::ranges::begin(vec), std::ranges::end(vec), std::greater<int>{}};
    auto q = cpc::prior_que{std::ranges::begin(vec), std::ranges::end(vec), std::less<int>{}};
    auto less = cpc::projection_helper<std::less<int>, std::identity, int, int>(std::less<int>{}, std::identity{});
    //cpc::trace_type(q);
    //int i0 = 2;
    //auto if0 = std::forward<int>(i0);
    //cpc::TypeTracer<decltype(if0)> x;
    //cpc::TypeTracer<decltype(less)> x;

    for (auto const& pq : q.data()) {
        std::cout << pq << std::endl;
    }
    auto result = less(vec[0], vec[1]);
    std::cout << "Projection result: " << result << std::endl;
}

void test() {
    std::vector v{{ 1, 5, 3, 1, 13, 21, 8 }};
    cpc::prior_que q{ v };
    std::cout << "test():" << "\n";

    std::cout << "size: " << q.size() << "\n";

    while (!q.empty()) {
        std::cout << q.top() << "\n";
        q.pop();
    }
    std::cout << "done." << std::endl;
}

int main(int, char**) {
    std::cout << std::boolalpha;
    test_projection();
    test0();
    test();

    return 0;
}
