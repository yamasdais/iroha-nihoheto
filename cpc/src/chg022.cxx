
#include "challenge.h"
#include "temperature.h"

void test0() {
    using namespace cpc::temperature;
    using namespace cpc::temperature::literal;

    auto t1 { 36.5_deg };
    auto t2 { 79.0_f };
    auto t3 { 42.3_k };

    auto tf = t1.convert<scale::fahrenheit>();
    auto tc = tf.convert<scale::celsius>();
    
    std::cout << std::boolalpha;
    std::cout << "t1: " << t1 << "\n"
        << "t2: " << t2 << "\n"
        << "t3: " << t3 << "(=" << t3.convert<scale::celsius>() << ")\n"
        << "tf: " << tf
        << std::endl;

    std::cout << "t1 == tc: " << (t1 == tc) << "\n"
        << "t2 <= tf: " << (t2 <= tf) << "\n"
        << "t2 > tf: " << (t2 > tf) << "\n"
        << std::endl;
}

int main(int, char**) {
    test0();
    return 0;
}
