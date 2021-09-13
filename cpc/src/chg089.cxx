#include <iostream>
#include <string>
#include <string_view>

#include "challenge.h"
#include "cryption.h"

void test0() {
    auto const text = "THECPPCHALLENGER";
    auto const key = "SAMPLE";
    auto enc = cpc::vigenere_encrypt(text, key);
    auto dec = cpc::vigenere_decrypt(enc, key);

    std::cout << "ORIG:" << text << "\n"
        << "ENC: " << enc << "\n"
        << "DEC: " << dec << std::endl;

}

int main(int, char**) {
    test0();
    return 0;
}