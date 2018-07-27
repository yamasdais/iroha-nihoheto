#include <cassert>
#include <string>
#include <iostream>
#include <array>
#include <typeinfo>
#include <typeindex>
#include <utility>

#include <boost/any.hpp>
#include <boost/hana.hpp>

#include "util.hpp"

namespace hana = boost::hana;

struct Fish {
  std::string name;
};

struct Cat {
  std::string name;
};

struct Dog {
  std::string name;
};

// hana 型の操作を値と同じような書き方で行えるような設計らしい
// メタプログラミングの複雑さ軽減（だが、型⇔値を行き来するので多少混乱する）
void test0() {
  using namespace hana::literals;
  auto animals = hana::make_tuple(
    Fish{"Nemo"},
    Cat{"Bigglesworth"},
    Dog{"Hati"}
  );
  // 1_c はユーザ定義リテラルで、コンパイル時定数
  const Cat dr_evils_pet = animals[1_c];

  std::cout << "Dr. Evil's pet:" << dr_evils_pet.name << std::endl;

  // value map
  auto names = hana::transform(animals, [](auto a) {
      return a.name;
    });
  assert(hana::reverse(names) == hana::make_tuple("Hati", "Bigglesworth", "Nemo"));
  std::cout << "names: " << T_name(names) << std::endl;

  // type map
  auto animal_types = hana::make_tuple(
    hana::type_c<Fish*>,
    hana::type_c<Cat&>,
    hana::type_c<Dog>
  );
  auto pointerless_types = hana::remove_if(animal_types, [](auto a) {
      return hana::traits::is_pointer(a);
    });

  static_assert(pointerless_types == hana::make_tuple(
                  hana::type_c<Cat&>,
                  hana::type_c<Dog>
                ));
  // static assert の代わり
  BOOST_HANA_CONSTANT_CHECK(animal_types[2_c] == hana::type_c<Dog>);
  static_assert(animal_types[0_c] == hana::type_c<Fish*>);

  // make_tuple の代わりに tuple_t<T...> と書いてもいいようだ
  BOOST_HANA_CONSTANT_CHECK(animal_types == hana::tuple_t<Fish*, Cat&, Dog>);

  // filter
  auto types = hana::tuple_t<int*, char&, void>;
  auto filtered_types = hana::filter(types, [](auto t) {
      using namespace hana::traits;
      return is_pointer(t) || is_reference(t);
    });
  std::cout << "types: " << T_name(filtered_types) << std::endl;

  // filter に渡している lambda の戻り値の型はなんだろう？
  std::cout << "lambda returns: "
            << T_name(hana::traits::is_pointer(types[0_c])) << std::endl;
  // => boost::hana::integral_constant<bool, true>

  auto has_name = hana::is_valid([](auto&& x) -> decltype((void)x.name) {});
  static_assert(has_name(dr_evils_pet));
  static_assert(!has_name(1));
}

struct Person {
  BOOST_HANA_DEFINE_STRUCT(
    Person,
    (std::string, name),
    (int, age)
  );
};

void test1() {
  constexpr auto serialize = [](std::ostream& os, auto const& object) {
    hana::for_each(hana::members(object), [](const auto& member) {
        std::cout << T_name(member) << std::end;
      });
  };

  const Person john{"John", 30};
  auto m = hana::members(john);
  std::cout << "members(john): " << T_name(m) << std::endl;
  hana::for_each(m, [](auto member) {
      std::cout << "foreach:" << member << std::endl;
    });
//  serialize(std::cout, john);
}

// switch sample
//[case]
#if false
template <typename T>
auto case_ = [](auto f) {
  return hana::make_pair(hana::type_c<T>, f);
};

struct default_t;
auto default_ = case_<default_t>;

// [process]
template <typename Any, typename Default>
auto process(Any&, std::type_index const& t, Default& default_) {
  return default_();
}

template <typename Any, typename Default, typename Case, typename... Rest>
auto process(Any& a, std::type_index const& t, Default& default_,
             Case& case_, Rest& ...rest)
{
  using T = typename decltype(+hana::first(case_))::type;
  return t == typeid(T)
  ? hana::second(case_)(*boost::unsafe_any_cast<T>(&a))
  : process(a, t, default_, rest...);
}
// [switch]
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = hana::make_tuple(cases_...);

    auto default_ = hana::find_if(cases, [](auto const& c) {
        return hana::first(c) == hana::type_c<default_t>;
      });
    static_assert(default_ != hana::nothing,
                  "switch is missing a default_ case");

    auto rest = hana::filter(cases, [](auto const& c) {
        return hana::first(c) != hana::type_c<default_t>;
      });

    return hana::unpack(rest, [&](auto& ...rest) {
        return process(a, a.type(), hana::second(*default_), rest...);
      });
  };
}
#endif

int main(int, char**) {
  test0();
  test1();
  
  return 0;
}
