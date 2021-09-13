#include <iostream>
#include <string>
#include <cctype>

#include "challenge.h"
#include "functional.h"

std::string caesar_encrypt(std::string_view text, int const shift) {
    return cpc::accum(text, std::string{}, [shift](std::string acc, auto ch) {
        acc += (std::isalpha(ch) && std::isupper(ch)) ? 'A' + (ch - 'A' + shift) % 26 : ch;
        return acc;
    });
}

std::string caesar_decrypt(std::string_view text, int const shift) {
    return cpc::accum(text, std::string{}, [shift](std::string acc, auto ch) {
        acc += (std::isalpha(ch) && std::isupper(ch)) ? 'A' + (26 + ch - 'A' - shift) % 26 : ch;
        return acc;
    });
}

void test0() {
    auto text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 1; i <= 26; i++) {
        auto enc = caesar_encrypt(text, i);
        auto dec = caesar_decrypt(enc, i);
        std::cout << dec << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}