#include <iostream>
#include <string>
#include <cctype>

#include "challenge.h"
#include "cryption.h"

void test0() {
    auto text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 1; i <= 26; i++) {
        auto enc = cpc::caesar_encrypt(text, i);
        auto dec = cpc::caesar_decrypt(enc, i);
        std::cout << dec << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}