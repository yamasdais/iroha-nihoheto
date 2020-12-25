#include <iomanip>
#include <stdexcept>

#include "challenge.h"
#include "str_util.h"

unsigned char hexchar_to_byte(unsigned char const ch)
    noexcept(false)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    throw std::invalid_argument("Invalid hexadecimal character");
}

std::vector<unsigned char> hexstr_to_bytes(std::string_view str) {
    bool isOdd = str.size() & 1;
    std::vector<unsigned char> ret;
    ret.reserve((str.size() >> 1) + isOdd ? 1 : 0);
    auto [begin, end] = cpc::toIters(str);
    if (isOdd && begin != end) {
        ret.push_back(hexchar_to_byte(*begin++));
    }
    for (; begin < end; begin += 2) {
        ret.push_back(hexchar_to_byte(*begin) << 4 | hexchar_to_byte(*(begin+1)));
    }
    return ret;
}

void solve0(std::string_view src) {
    auto str0 = hexstr_to_bytes(src);
    std::cout << std::hex;
    for (auto c : str0) {
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(c) << ", ";
    }
    std::cout << std::endl;
}

void solve1(char const* src) {
    for (auto& cell : cpc::split({src}, ' ')) {
        //std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(c) << ", ";
        //std::cout << cell << std::endl;
        solve0(cell);
    }
}

int main(int, char**) {
    //auto str0 = hexstr_to_bytes("BAD");
    solve0("BAADF00D42");
    solve1("BA AD F0 d4");
    return 0;
}

