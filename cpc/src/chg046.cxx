#include <array>

#include "challenge.h"
#include "ring_buffer.h"

void test0() {
    using cont_type = std::vector<int>;
    cpc::ring_buffer<int> buf{cont_type(5)};
    std::cout << "ring_buffer<int>" << "\n";
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
}

void test_array() {
    using cont_type = std::array<int, 5>;
    cpc::ring_buffer<int, cont_type> buf{};
    cpc::ring_buffer<int, std::array<int, 0>> buf_e{};
    std::cout << "ring_buffer<int, std::array<int, 5>>" << "\n";
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
    //auto itr = buf.begin();
    //cpc::TypeTracer<decltype(itr)::const_reference> x0;
    //cpc::TypeTracer<decltype(buf)::const_iterator> x;
    //buf_e.push(2);
    //buf.push(2);
    //cpc::TypeTracer<decltype(buf)::foo> x;
    //constexpr auto x = cpc::tt<decltype(buf)::container_type>();
    //using is_c = cpc::is_empty_constexpr<decltype(buf)::container_type>::type;
    //using emp_type = decltype(&cont_type::empty);
    //using em = decltype(cpc::is_member_constexpr_concl<cont_type, &cont_type::empty>(0));
    //cpc::TypeTracer<em> t0;
    //using aa1 = cpc::check_m_empty_constexpr<cont_type>;
    //cpc::TypeTracer<aa1> y;
    //using aa0 = decltype(cpc::detect_constexpr_invocable<[]() { return cont_type{}.empty(); }>(0));
    //using is_c = cpc::is_empty_constexpr<cont_type>;
    //cpc::TypeTracer<is_c> y;
}

int main(int, char**) {
    test0();
    test_array();
    return 0;
}
