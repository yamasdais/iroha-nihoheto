#include <cassert>
#include <string>
#include <iostream>

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
  Cat dr_evils_pet = animals[1_c];

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
}

int main(int, char**) {
  test0();
  
  return 0;
}
