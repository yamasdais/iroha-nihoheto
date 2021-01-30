#include <array>

#include "challenge.h"
#include "ring_buffer.h"

void test0() {
    std::vector<int> v;
    using cont_type = std::vector<int>;
    cpc::ring_buffer<int> buf{24};
    //cpc::TypeTracer<decltype(buf)::foo> x;
    //using aa1 = cpc::check_m_empty_constexpr<cont_type>;
    //cpc::TypeTracer<aa1> y;
    //cpc::TypeTracer<&v.empty> x;
    //cpc::TypeTracer<decltype(&std::vector<int>::empty)> x;
    //
    using is_c = cpc::is_empty_constexpr<std::vector<int>>;
    cpc::TypeTracer<is_c> y;
}

void test_array() {
    cpc::ring_buffer<int, std::array<int, 12>> buf{};
    using cont_type = std::array<int, 12>;
    //cpc::TypeTracer<decltype(buf)::foo> x;
    //constexpr auto x = cpc::tt<decltype(buf)::container_type>();
    //using is_c = cpc::is_empty_constexpr<decltype(buf)::container_type>::type;
    //using emp_type = decltype(&cont_type::empty);
    //using em = decltype(cpc::is_member_constexpr_concl<cont_type, &cont_type::empty>(0));
    //cpc::TypeTracer<em> t0;
    //using aa1 = cpc::check_m_empty_constexpr<cont_type>;
    //cpc::TypeTracer<aa1> y;
    //using aa0 = decltype(cpc::detect_constexpr_invocable<[]() { return cont_type{}.empty(); }>(0));

    using is_c = cpc::is_empty_constexpr<cont_type>;
    cpc::TypeTracer<is_c> y;
}

int main(int, char**) {
    test0();
    test_array();
    return 0;
}
