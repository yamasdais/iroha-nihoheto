#include <array>

#include "challenge.h"
#include "ring_buffer.h"

void test0() {
    //using cont_type = std::vector<int>;
    //cpc::ring_buffer<int> buf{cont_type(5)};
    cpc::ring_buffer<int> buf(5u);
    std::cout << "ring_buffer<int>\n";
    buf.push(1);
    buf.push(2);
    buf.push(3);
    [[maybe_unused]] auto item = buf.pop();
    buf.push(4);
    //cpc::TypeTracer<decltype(buf)::foo> x;
    //using aa1 = cpc::check_m_empty_constexpr<cont_type>;
    //cpc::TypeTracer<aa1> y;
    //cpc::TypeTracer<&v.empty> x;
    //cpc::TypeTracer<decltype(&std::vector<int>::empty)> x;
    //
    //using is_c = cpc::is_empty_constexpr<cont_type>;
    //cpc::TypeTracer<is_c> y;
    std::cout << "ring_buffer<int>: initializer_list\n";
    cpc::ring_buffer<int> buf0{ 0, 1, 2, 3 };
    item = buf0.pop();
    buf0.push(4);
}

void test_array() {
    using cont_type = std::array<int, 5>;
    cpc::ring_buffer<int, cont_type> buf;
    cpc::ring_buffer<int, std::array<int, 0>> buf_e{};
    std::cout << "ring_buffer<int, std::array<int, 5>>\n";
    buf.push(1);
    buf.push(2);
    buf.push(3);
    [[maybe_unused]] auto item = buf.pop();
    buf.push(4);
    buf.push(5);
    buf.push(6);
    buf.push(7);
    buf.push(8);
    item = buf.pop();
    item = buf.pop();
    item = buf.pop();
    item = buf.pop();
    item = buf.pop();
    buf.push(9);
    std::cout << "ring_buffer<int, std::array<int, 5>>: initializer_list\n";
    cpc::ring_buffer<int, cont_type> buf0{ 0, 1, 2, 3, 4, 5 };
    buf0.push(99);
}

int main(int, char**) {
    test0();
    test_array();
    return 0;
}
