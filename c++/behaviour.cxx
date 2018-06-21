#include <iostream>

#include "util.hpp"

void behaviour0() {
  constexpr auto utf8_char = u8"あ";
  std::cout << T_name(utf8_char) << ": " << utf8_char << std::endl;
  using TNameType = decltype(T_name(utf8_char));
  std::cout << "typeindex::type_id_with_cvrが返す型は？ "
            << T_name(T_name(1)) << "(size:" << sizeof(TNameType) << ")"
            << std::endl;
  // => std::string だった
}

int main(int, char**) {
  behaviour0();
  return 0;
}
