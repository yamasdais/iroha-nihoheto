#include <iostream>
#include <string>
#include <png++/png.hpp>

#include "challenge.h"

void createFlangPng(std::string filepath, uint32_t width, uint32_t height) {
    png::image<png::rgb_pixel> image{width, height};
    size_t red_width = width / 3;
    size_t yellow_width = width * 2 / 3;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (x < red_width)
                image[y][x] = png::rgb_pixel(0xff, 0x00, 0x00);
            else if (x < yellow_width)
                image[y][x] = png::rgb_pixel(0xff, 0xff, 0x00);
            else
                image[y][x] = png::rgb_pixel(0x00, 0x00, 0xff);
        }
    }
    image.write(filepath);
}

void test0() {
    try {
        createFlangPng("../data/romania.png", 260, 180);

    } catch (png::error& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}