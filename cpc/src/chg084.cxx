#include <string>
#include <string_view>
#include <array>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <iostream>

#include <png++/png.hpp>

#include "challenge.h"
#include "functional.h"

struct ean13 {
    explicit ean13(std::string const& code)
    : number(genNumber(code)) {
    }
    explicit ean13(uint64_t code)
    : ean13(std::to_string(code)) {
    }

    std::string to_string() const noexcept {
        return number;
    }

    std::array<unsigned char, 13> to_array() const {
        std::array<unsigned char, 13> result;
        std::ranges::for_each(std::ranges::iota_view(0u, 13u),
            [&result, this](auto const i) {
                result[i] = static_cast<unsigned char>(number[i] - '0');
            });
        return result;
    }
private:
    static std::string genNumber(std::string const& code) {
        if (code.length() == 13) {
            if (code[12] != '0' + get_crc(code.substr(0, 12)))
                throw std::runtime_error("Not an EAN-13 format.");
            return code;
        } else if (code.length() == 12) {
            return code + std::string(1, '0' + get_crc(code));
        }
        throw std::runtime_error("Not an EAN-13 format.");
    }
    static unsigned char get_crc(std::string_view code) {
        unsigned char weights[12] = { 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3 };
        size_t index = 0u;
        auto const sum = cpc::accum(code, 0u,
        [&weights, &index](int const total, char const c) {
            return total + weights[index++] * (c - '0');
        });
        return static_cast<unsigned char>(10 - sum % 10);
    }

    std::string number;
};

class ean13_barcode_generator {
    int draw_digit(unsigned char const code, unsigned int const size,
    png::image<png::rgba_pixel>& image, int x, int y,
    int const digit_width, int const height) const {
        std::bitset<7> const bits(code);
        for (int i = size - 1; i >= 0; i--) {
            if (bits[i] != 0)
            for (int h = 0; h < height; h++) {
                for (int v = x, vend = x + digit_width; v < vend; v++) {
                    image[y+h][v] = png::rgba_pixel(0x00, 0x00, 0x00);
                }
            }
            x += digit_width;
        }

        return x;
    }

    unsigned char const encodings[10][3] = {
        { 0b0001101, 0b0100111, 0b1110010 }, // 0
        { 0b0011001, 0b0110011, 0b1100110 }, // 1
        { 0b0010011, 0b0011011, 0b1101100 }, // 2
        { 0b0111101, 0b0100001, 0b1000010 }, // 3
        { 0b0100011, 0b0011101, 0b1011100 }, // 4
        { 0b0110001, 0b0111001, 0b1011100 }, // 5
        { 0b0101111, 0b0000101, 0b1010000 }, // 6
        { 0b0111011, 0b0010001, 0b1000100 }, // 7
        { 0b0110111, 0b0001001, 0b1001000 }, // 8
        { 0b0001011, 0b0010111, 0b1110100 }, // 9
    };
    unsigned char const eandigits[10][6] = {
        { 0, 0, 0, 0, 0, 0}, // 0
        { 0, 0, 1, 0, 1, 1}, // 1
        { 0, 0, 1, 1, 0, 1}, // 2
        { 0, 0, 1, 1, 1, 0}, // 3
        { 0, 1, 0, 0, 1, 1}, // 4
        { 0, 1, 1, 0, 0, 1}, // 5
        { 0, 1, 1, 1, 0, 0}, // 6
        { 0, 1, 0, 1, 0, 1}, // 7
        { 0, 1, 0, 1, 1, 0}, // 8
        { 0, 1, 1, 0, 1, 0}, // 9
    };

    unsigned char const marker_start = 0b101;
    unsigned char const marker_end = 0b101;
    unsigned char const marker_center = 0b01010;

  public:
    void create(ean13 const& code, std::string const& filename, uint32_t const digit_width = 3,
        uint32_t const height = 50, uint32_t margin = 10) const
    {
        png::image<png::rgba_pixel> image{margin * 2 + 95 * digit_width, height + margin * 2};
        std::array<unsigned char, 13> const digits = code.to_array();
        int x = margin;
        x = draw_digit(marker_start, 3, image, x, margin, digit_width, height);
        for (int i = 0; i < 6; ++i) {
            int const code = encodings[digits[1+i]][eandigits[digits[0]][i]];
            x = draw_digit(code, 7, image, x, margin, digit_width, height);
        }
        x = draw_digit(marker_center, 5, image, x, margin, digit_width, height);
        for (int i = 0; i < 6; ++i) {
            int const code = encodings[digits[7+i]][2];
            x = draw_digit(code, 7, image, x, margin, digit_width, height);
        }
        x = draw_digit(marker_end, 5, image, x, margin, digit_width, height);

        image.write(filename);
    }
};

void test0() {
    ean13 eanCode{"5901234123457"};
    std::cout << "Code: " << eanCode.to_string() << std::endl;
    ean13_barcode_generator const generator;
    generator.create(eanCode, "../data/5901234123457.png", 5, 150, 30);
}

int main(int, char**) {
    try {
        test0();
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
    return 0;
}