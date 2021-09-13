#pragma once

#include <string>
#include <string_view>
#include <ranges>

#include "challenge.h"
#include "functional.h"

namespace challenge100 {

std::string caesar_encrypt(std::string_view text, int const shift) {
    std::string ret;
    ret.reserve(text.length());
    return accum(text, std::move(ret), [shift](std::string acc, auto ch) {
        acc += (std::isalpha(ch) && std::isupper(ch)) ? 'A' + (ch - 'A' + shift) % 26 : ch;
        return acc;
    });
}

std::string caesar_decrypt(std::string_view text, int const shift) {
    std::string ret;
    ret.reserve(text.length());
    return accum(text, std::move(ret), [shift](std::string acc, auto ch) {
        acc += (std::isalpha(ch) && std::isupper(ch)) ? 'A' + (26 + ch - 'A' - shift) % 26 : ch;
        return acc;
    });
}

std::string build_vigenere_table() {
    std::string table;
    table.reserve(26 * 26);
    std::ranges::for_each(std::ranges::iota_view(0u, 26u), [&table](uint32_t i) {
        table += caesar_encrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZ", i);
    });

    return table;
}

std::string vigenere_encrypt(std::string_view text, std::string_view key) {
    std::string ret;
    ret.reserve(text.length());
    std::ranges::for_each(std::ranges::iota_view(0u, text.length()),
        [&text, &key, &ret, table=build_vigenere_table(), keyLen=key.length()](uint32_t const i) {
        auto row = key[i % keyLen] - 'A';
        auto col = text[i] - 'A';
        ret += table[row * 26 + col];
    });

    return ret;
}

std::string vigenere_decrypt(std::string_view text, std::string_view key) {
    std::string ret;
    ret.reserve(text.length());
    std::ranges::for_each(std::ranges::iota_view(0u, text.length()),
        [&text, &key, &ret, table=build_vigenere_table(), keyLen=key.length()](uint32_t const i) {
        auto row = key[i % keyLen] - 'A';
        for (size_t col = 0; col < 26; col++) {
            if (table[row * 26 + col] == text[i]) {
                ret += 'A' + col;
                break;
            }
        }
    });

    return ret;

}

}