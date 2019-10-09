#include <iostream>
#include <utility>

#include "util.hpp"

template <typename ToC, typename FromC>
struct CastChar {
  template <FromC Value>
  constexpr static ToC apply =
  static_cast<ToC>(static_cast<std::make_unsigned_t<FromC>>(Value));
};


template <typename C>
struct CastChar<C, C> {
  template <C Value>
  constexpr static C apply = (C)2;
};

void behaviour0() {
  constexpr auto utf8_char = u8"あ";
  std::cout << T_name(utf8_char) << ": " << utf8_char << std::endl;
  using TNameType = decltype(T_name(utf8_char));
  std::cout << "typeindex::type_id_with_cvrが返す型は？ "
            << T_name(T_name(1)) << "(size:" << sizeof(TNameType) << ")"
            << std::endl;
  // => std::string だった
}


void b1() {
  using a = CastChar<wchar_t, char>;
  using b = CastChar<char, char>;
  constexpr auto c = a::template apply<0x80>;

  std::cout << "b1: " << T_name<a>() << std::endl;
  std::cout << "c: " << T_name(c) << ", " << (int)c << std::endl;
  std::cout << "C: " << (int)static_cast<wchar_t>((char)0x80) << std::endl;
  std::cout << "cb: " << (int)b::apply<0x80> << std::endl;
}

int main(int, char**) {
  behaviour0();
  b1();
  return 0;
}
