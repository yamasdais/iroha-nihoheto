#include <chrono>

#include "challenge.h"

// libstdc++ にはまだ chrono::day 等が定義されてなかった
#if 0
int number_of_days(int const y0, unsigned int const m0, unsigned int const d0,
        int const y1, unsigned int const m1, unsigned int const d1) {
    using namespace std::chrono;
    return (sys_days{ year{y0} / month{m0} / day{d0} }
            - sys_days{ year{y1}/ month{m1} / day{d0} }).count();
    using namespace std::literals;
    return 0;
}
#endif

int main(int, char**) {
    return 0;
}
