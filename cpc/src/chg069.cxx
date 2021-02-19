#include <iostream>
#include <iomanip>
#include <ranges>
#include <concepts>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <functional>

#include "challenge.h"
#include "random.h"
#include "functional.h"

enum class gender_type {
    male,
    female
};

template <class Derived>
class social_number_generator {
  protected:
    int gender_code(gender_type gc) const {
        return static_cast<Derived const*>(this)->gender_code(gc);
    }
    int next_rand(uint16_t const year, uint16_t const month, uint16_t const dom) {
        return static_cast<Derived*>(this)->next_rand(year, month, dom);
    }
    int modulo_value() const {
        return static_cast<Derived const*>(this)->modulo_value();
    }

    using rand_key_type = unsigned int;
    using rand_gen_type = cpc::random_generator<std::uniform_int_distribution<int>>;
    std::map<rand_key_type, std::set<int>> cache;

    int gen_rand(uint16_t const year, uint16_t const month, uint16_t const dom, rand_gen_type& randomize) {
        rand_key_type key = year * 10'000 + month * 100 + dom;
        do {
            auto const num = randomize();
            if (auto const d_pos{cache.find(key)}; d_pos == std::end(cache)) {
                cache[key] = std::set<int>{ num };
                return num;
            } else if (auto& nums{d_pos->second}; nums.find(num) == std::end(nums)) {
                nums.emplace(num);
                return num;
            }
        } while (true);
    }

  public:
    std::string generate(gender_type gender, uint16_t const year, uint16_t const month, uint16_t const dom)
    {
        std::stringstream snumber;

        snumber << gender_code(gender);
        snumber << year << std::setw(2) << std::setfill('0') << month
           << std::setw(2) << std::setfill('0') << dom;
        snumber << next_rand(year, month, dom);

        auto const number = snumber.str();
        auto index = number.length();
        auto const sum = cpc::accum(number, 0u,
                [&index](uint32_t const acc, char const c) {
                return acc + static_cast<uint32_t>(index-- * (c - '0'));
                });

        auto const rest = sum % modulo_value();
        snumber << modulo_value() - rest;

        return snumber.str();
    }

};

struct southeria_social_number_generator : public social_number_generator<southeria_social_number_generator> {
    southeria_social_number_generator()
        : social_number_generator{}
    {}

    constexpr int gender_code(gender_type gc) const {
        return gc == gender_type::male ? 2 : 1;
    }

    int next_rand(uint16_t const year, uint16_t const month, uint16_t const dom) {
        rand_gen_type rg{1000, 9999};
        return gen_rand(year, month, dom, rg);
    }

    constexpr int modulo_value() const {
        return 10;
    }
};

struct northeria_social_number_generator : public social_number_generator<northeria_social_number_generator> {
    northeria_social_number_generator()
        : social_number_generator{}
    {}

    constexpr int gender_code(gender_type gc) const {
        return gc == gender_type::male ? 7 : 9;
    }

    int next_rand(uint16_t const year, uint16_t const month, uint16_t const dom) {
        rand_gen_type rg{10000, 99999};
        return gen_rand(year, month, dom, rg);
    }

    constexpr int modulo_value() const {
        return 11;
    }
};

enum class country_type {
    northeria,
    southeria,
};

struct social_number_generator_factory {
    using function_type = std::function<std::string(gender_type const, uint16_t const, uint16_t const, uint16_t const)>;
    function_type get_generator(country_type country) {
        if (country == country_type::northeria)
            return {[this](gender_type const gender, uint16_t year, uint16_t month, uint16_t dom) {
                return northeria.generate(gender, year, month, dom);
            }};
        if (country == country_type::southeria)
            return {[this](gender_type const gender, uint16_t year, uint16_t month, uint16_t dom) {
                return southeria.generate(gender, year, month, dom);
            }};

        return {};
    }
  private:
    northeria_social_number_generator northeria;
    southeria_social_number_generator southeria;
};

void test0() {
    auto factory = social_number_generator_factory{};
    auto north_generate = factory.get_generator(country_type::northeria);
    auto south_generate = factory.get_generator(country_type::southeria);

    auto sn0 = north_generate(gender_type::female, 2017, 12, 25);
    std::cout << "sn0:" << sn0 << std::endl;
    auto sn1 = north_generate(gender_type::female, 2017, 12, 25);
    std::cout << "sn1:" << sn1 << std::endl;
    auto sn2 = north_generate(gender_type::male, 2017, 12, 25);
    std::cout << "sn2:" << sn2 << std::endl;

    auto ss0 = south_generate(gender_type::female, 2017, 12, 25);
    std::cout << "ss0:" << ss0 << std::endl;
    auto ss1 = south_generate(gender_type::female, 2017, 12, 25);
    std::cout << "ss1:" << ss1 << std::endl;
    auto ss2 = south_generate(gender_type::male, 2017, 12, 25);
    std::cout << "ss2:" << ss2 << std::endl;

}

int main(int, char**) {
    test0();

    return 0;
}
