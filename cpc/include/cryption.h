#pragma once

#include <string>
#include <string_view>
#include <ranges>

#include "challenge.h"
#include "functional.h"

namespace challenge100 {

// Caesar
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

// Vigenere
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

// Base 64
class base64 {
   inline static std::string const table_enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   inline static constexpr char const padding_symbol = '=';
   
   inline static constexpr char const table_dec[256] =
   {
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
      52,53,54,55,56,57,58,59,60,61,-1,-1,-1,65,-1,-1,
      -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
      15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
      -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
      41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
   };
   inline static constexpr char const invalid_char = -1;
   inline static constexpr char const padding_char = 65;

  public:
    std::string encode(std::vector<u_char> const& data) const {
        std::string ret;
        ret.resize((data.size()/3 + ((data.size()%3 > 0) ? 1 : 0)) * 4);
        auto retIter = std::ranges::begin(ret);
        size_t i{0}, j{0};
        while (j++ < data.size() / 3) {
            u_int32_t value = (data[i] << 16) | (data[i+1] << 8) | data[i+2];
            *retIter++ = table_enc[(value & 0xfc0000) >> 18];
            *retIter++ = table_enc[(value & 0x03f000) >> 12];
            *retIter++ = table_enc[(value & 0x000fc0) >> 6];
            *retIter++ = table_enc[(value & 0x00003f)];
            i += 3;
        }
        auto rest = data.size() - i;
        if (rest == 1) {
            *retIter++ = table_enc[(data[i] & 0x0000fc) >> 2];
            *retIter++ = table_enc[(data[i] & 0x000003) << 4];
            *retIter++ = padding_symbol;
            *retIter++ = padding_symbol;
        } else if (rest == 2) {
            uint32_t value = (data[i] << 8) | data[i+1];
            *retIter++ = table_enc[(value & 0x00fc00) >> 10];
            *retIter++ = table_enc[(value & 0x0003f0) >> 4];
            *retIter++ = table_enc[(value & 0x00000f) << 2];
            *retIter++ = padding_symbol;
        }

        return ret;
    }

    std::vector<u_char> decode(std::string data) const {
        size_t padding = data.size() % 4;
        if (padding == 0) {
            if (data[data.size() - 1] == padding_symbol) padding++;
            if (data[data.size() - 2] == padding_symbol) padding++;
        } else {
            data.append(2, padding_symbol);
        }

        std::vector<u_char> ret;
        ret.resize((data.length() / 4) * 3 - padding);
        auto resPtr = std::ranges::begin(ret);
        size_t i{0}, j{0};
        while (j++ < data.size() / 4) {
            u_char c1 = table_dec[static_cast<int>(data[i++])];
            u_char c2 = table_dec[static_cast<int>(data[i++])];
            u_char c3 = table_dec[static_cast<int>(data[i++])];
            u_char c4 = table_dec[static_cast<int>(data[i++])];
            if (c1 == invalid_char || c2 == invalid_char || c3 == invalid_char || c4 == invalid_char)
            throw std::runtime_error("invalid base64 encoding");

            if (c4 == padding_char) {
                if (c3 == padding_char) {
                    uint32_t value = (c1 << 6) | c2;
                    *resPtr++ = (value >> 4) & 0x0000ff;
                } else {
                    uint32_t value = (c1 << 12) | (c2 << 6) | c3;
                    *resPtr++ = (value >> 10) & 0x0000ff;
                    *resPtr++ = (value >> 2) & 0x0000ff;
                }
            } else {
                uint32_t value = (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;
                *resPtr++ = (value >> 16) & 0x0000ff;
                *resPtr++ = (value >> 8) & 0x0000ff;
                *resPtr++ = value & 0x0000ff;
            }
        }

        return ret;
    }
};


}