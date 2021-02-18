#include <iostream>
#include <vector>
#include <memory>
#include <ranges>
#include <concepts>
#include <random>
#include <type_traits>

#include "challenge.h"
#include "functional.h"
#include "random.h"

struct password_generator {
    virtual std::string generate() = 0;
    virtual void add(std::unique_ptr<password_generator>) {
    }
};

template <auto func>
    requires std::is_nothrow_invocable_r_v<std::string_view, decltype(func)>
struct basic_password_generator : public password_generator {
    basic_password_generator(size_t const len)
        : len{len}
    { }

    std::string generate() {
        constexpr auto allowed = std::invoke(func);
        cpc::random_generator<std::uniform_int_distribution<int>> rg{0, allowed.length() - 1};
        std::string ret;
        for ([[maybe_unused]] auto i : std::views::iota(0u, len)) {
            ret.push_back(allowed[rg()]);
        }
        return ret;
    }
  private:
    size_t const len;
};

struct composite_password_generator : public password_generator {
    composite_password_generator()
        : generators{}
    {}

    void add(std::unique_ptr<password_generator> generator) {
        generators.push_back(std::move(generator));
    }

    std::string generate() {
        std::string ret = cpc::accum(generators, std::string{},
                [](std::string acc, std::unique_ptr<password_generator> const& generator) {
                return acc + generator->generate();
                });
        std::ranges::shuffle(ret, cpc::detail::mt19937engine());
        return ret;
    }

  private:
    std::vector<std::unique_ptr<password_generator>> generators;
};

using digit_generator = basic_password_generator<[]() noexcept {
    return std::string_view("0123456789");
}>;

using symbol_generator = basic_password_generator<[]() noexcept {
    return std::string_view("!@#$%^&*_+");
}>;

using upper_letter_generator = basic_password_generator<[]() noexcept {
    return std::string_view("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}>;

using lower_letter_generator = basic_password_generator<[]() noexcept {
    return std::string_view("abcdefghijklmnopqrstuvwxyz");
}>;

void test0() {
    composite_password_generator generator;
    generator.add(std::make_unique<digit_generator>(3));
    generator.add(std::make_unique<symbol_generator>(1));
    generator.add(std::make_unique<upper_letter_generator>(2));
    generator.add(std::make_unique<lower_letter_generator>(4));

    auto password = generator.generate();
    std::cout << "Password: " << password << std::endl;
}

int main(int, char**) {
    test0();

    return 0;
}
/*
 * 書籍のような実装だと composite と leaf でメソッドがバラバラなので、
 * あんまり composite っぽさが感じられない。
 * この様な実装の方が Composite pattern らしいと思う
 */
