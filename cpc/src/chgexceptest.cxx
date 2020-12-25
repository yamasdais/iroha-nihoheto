#include "challenge.h"

#include "exception_util.h"

struct dumb_thrower {
    int n;
    [[no_unique_address]] cpc::exception_rethrow_enabler<false> thrower;
};

struct real_thrower {
    int n;
    [[no_unique_address]] cpc::exception_rethrow_enabler<true> thrower;
};

void thrower() {
    std::cout << "thrower()" << std::endl;
    throw std::runtime_error("runtime error!");
}

void func0()
    noexcept(false)
{
    dumb_thrower rethrower;
    try {
        std::cout << "size of dumb_thrower: " << sizeof(dumb_thrower) << "\n"
            << "size of real_thrower: " << sizeof(real_thrower) << std::endl;
        thrower();
    } catch (std::exception& e) {
        std::cout << "handling exception in func0()" << std::endl;
        rethrower.thrower.assign(std::current_exception());
    }
    rethrower.thrower.rethrow();
}


int main(int, char**) {
    try {
        func0();
    } catch (std::exception& e) {
        std::cout << "catch in main: " << e.what() << std::endl;
    }
    return 0;
}
