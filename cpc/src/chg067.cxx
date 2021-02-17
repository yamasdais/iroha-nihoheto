#include <string>
#include <string_view>
#include <memory>
#include <iostream>
#include <concepts>
#include <ranges>
#include <cctype>

#include "challenge.h"

struct password_validator {
    virtual bool validate(std::string_view) const = 0;
    virtual ~password_validator() = default;
};

struct length_validator final : public password_validator {
    explicit length_validator(uint16_t min_length)
        : password_validator{}, length{min_length}
    {}

    virtual bool validate(std::string_view password) const override {
        return password.length() >= length;
    }

  private:
    uint16_t const length;
};

template <auto func>
    requires std::predicate<decltype(func), std::string_view>
struct password_validator_decorator : public password_validator {
    explicit password_validator_decorator(std::unique_ptr<password_validator> validator)
        : password_validator{}, inner{std::move(validator)}
    {}

    virtual bool validate(std::string_view password) const override {
        return inner->validate(password) && std::invoke(func, password);
    }

  private:
    std::unique_ptr<password_validator> inner;
};

using digit_password_validator = password_validator_decorator<[](std::string_view password) {
    return password.find_first_of("0123456789") != std::string::npos;
}>;

using case_password_validator = password_validator_decorator<[](std::string_view password) {
    return std::ranges::any_of(password, [](auto const c) { return std::islower(c); })
        && std::ranges::any_of(password, [](auto const c) { return std::isupper(c); });
}>;

using symbol_password_validator = password_validator_decorator<[](std::string_view password) {
    return password.find_first_of("!@#$%^&*?") != std::string::npos;
}>;

auto test_validate = [](std::string_view name, auto const& validator, std::string_view password)
{
    std::cout << name << " for " << password <<  " : " << validator->validate(password) << "\n";
};

void test0() {
    auto validate1 = std::make_unique<digit_password_validator>(
            std::make_unique<length_validator>(8));
    test_validate("validate1", validate1, "abc123!@#");
    test_validate("validate1", validate1, "abc1!@#");
    test_validate("validate1", validate1, "abcd!@#&x");

    auto validate2 = std::make_unique<symbol_password_validator>(
            std::make_unique<case_password_validator>(
                std::make_unique<digit_password_validator>(
                    std::make_unique<length_validator>(8))));
    test_validate("validate2", validate2, "Abc1!@#&x");
    test_validate("validate2", validate2, "abcd!@#&x");
}

int main(int, char**) {
    std::cout << std::boolalpha;
    test0();

    return 0;
}

/*
 * validate() メソッドじゃなくて operator() を仮想関数化した方が良いように思う
 */
