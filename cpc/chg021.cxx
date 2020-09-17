#include <cstdio>
#include <memory>
#include <stdexcept>

#include "challenge.h"

struct invalid_value {
    std::FILE* operator()() {
        return nullptr;
    }
};

struct exception_if_fail {
    std::FILE* operator()() {
        throw std::runtime_error("failed!");
    }
};

struct fp_cleaner {
    void operator()(FILE* fp) {
        std::cout << "closing file pointer" << std::endl;
        std::fclose(fp);
    }
};

int main(int, char**) {
    constexpr char const* filename = "../README.md";
    auto fp = std::unique_ptr<std::FILE, fp_cleaner>{
        std::fopen(filename, "r")
    };
    if (fp.get() == nullptr) {
        std::cout << "failed to open file" << std::endl;
    }

    char ch;
    while ((ch = std::fgetc(fp.get())) != EOF) {
        std::putchar(ch);
    }

    return 0;
}
