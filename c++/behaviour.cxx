#include <boost/type_index.hpp>
#include <iostream>

template <typename T>
auto T_name()
  noexcept(noexcept(boost::typeindex::type_id_with_cvr<T>().pretty_name()))
{
  return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

template <typename T>
auto T_name(T)
  noexcept(noexcept(T_name<T>()))
{
  return T_name<T>();
}

void behaviour0() {
  constexpr auto utf8_char = u8"あ";
  std::cout << T_name(utf8_char) << ": " << utf8_char << std::endl;
  using TNameType = decltype(T_name(utf8_char));
  std::cout << "typeindex::type_id_with_cvrが返す型は？ "
            << T_name(T_name(1)) << "(size:" << sizeof(TNameType) << ")"
            << std::endl;
}

int main(int, char**) {
  behaviour0();
  return 0;
}
