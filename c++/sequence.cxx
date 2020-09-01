#include <concepts>
#include <iostream>
#include <iterator>

template <class T, class Pred = std::unreachable_sentinel_t>
class sequence {
  protected:
  public:
    sequence() {}
};

int main(int, char**) {
    auto s = sequence<int>{};
    return 0;
}
