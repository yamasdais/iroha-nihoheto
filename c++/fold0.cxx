#include <iostream>

template <typename T>
auto prnt_(T&& v)
{
  return std::forward<T>(v);
}

// 非テンプレートの関数でもオーバーロードできるのか。
#if false
auto prnt_(std::string&& v) {
  return std::string{'['} + v + std::string{']'};
}
#endif

template <typename... T>
void prnt_im(T&&... args)
{
  (std::cout << ... << prnt_(std::forward<T>(args)));
}

template <typename... T>
void prnt(T&&... args)
{
  prnt_im(std::forward<T>(args)...);
}

int main(int, char**)
{
  std::string strvar = "ABC";
  prnt(1, 42, 2.768, std::string{"abc"}, strvar);
  return 0;
}
