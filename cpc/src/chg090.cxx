#include <iostream>
#include <string>
#include <algorithm>

#include "challenge.h"
#include "cryption.h"

std::vector<u_char> to_vector(std::string_view data) {
    std::vector<u_char> ret;
    ret.reserve(data.length());

    std::copy(std::ranges::cbegin(data), std::ranges::cend(data),
              std::back_inserter(ret));

    return ret;
}

std::string to_string(std::vector<u_char> const& data) {
    std::string ret;
    ret.reserve(data.size());

    std::copy(std::ranges::cbegin(data), std::ranges::cend(data),
              std::back_inserter(ret));

    return ret;
}

void test0() {
    cpc::base64 encoder;
    std::vector<u_char> const data = { 'c', 'p', 'p' };
    auto text = "cppchallenge";
    auto enc = encoder.encode(to_vector(text));
    auto dec = encoder.decode(enc);

    std::cout << "ENC: " << enc << std::endl;
    std::cout << "DEC: " << to_string(dec) << std::endl;

}

int main(int, char**) {
    test0();
    return 0;
}