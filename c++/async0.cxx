#include <iostream>
#include <fstream>
#include <vector>
#include <future>

#include "util.hpp"

std::vector<char> readFile(std::string fname)
{
  std::ifstream is{fname, std::ios::in | std::ios::binary};
  if (!is) {
    throw std::runtime_error(std::string{"Cannot open file: "} + fname);
  }
  is.seekg(0, std::ios::end);
  auto fsize = is.tellg();
  std::cout << "File size: " << fsize << std::endl;
  is.seekg(0, std::ios::beg);

  // std::vector は initializer_list をとるコンストラクタが定義されているので、
  // ret{fsize} と波括弧で生成しようとしても、それは初期化リストと解釈される。
  std::vector<char> ret(fsize);
  is.read(ret.data(), fsize);
  return ret;
}

int main(int argc, const char* argv[]) {
  if (argc > 1) {
    try {
      auto ftr = std::async(
        std::launch::async,
        readFile,
        std::string(argv[1])
      );
      std::cout << "created: async instance: "
//                << T_name<decltype(ftr)>()
                << T_name(ftr)
                << std::endl;
      // block される非同期呼び出し
      auto buf = ftr.get();

      for (auto c : buf) {
        std::cout << c;
      }
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
    }
  } else {
    std::cerr << "Usage: async0 <filename>" << std::endl;
  }
  return 0;
}
