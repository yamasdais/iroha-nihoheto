#include <ranges>
#include <regex>
#include <filesystem>

#include "challenge.h"
#include "coro_util.h"

cpc::generator<std::string, true> find_files(std::string const& path) {
    namespace fs = std::filesystem;
    for (fs::recursive_directory_iterator i{path}, end; i != end; ++i) {
        co_yield i->path();
    }
}

void list_files_regex(std::string const& pattern) {
    std::regex rx(pattern);
    auto files_co = find_files(".");
    auto filter_re = [re=std::move(rx)](auto const& fname) {
        return std::regex_search(fname, re);
    };

    std::ranges::for_each(files_co | std::views::filter(std::move(filter_re)),
            [](std::string const& fname) {
                std::cout << fname << "\n";
            });
}

int main(int argc, char* argv[]) {
    // 検索ルートディレクトリを指定できるようにコマンドラインオプションを追加しても良いかもしれない
    if (argc < 2) {
        std::cout << "Usage: chg037 <regex for filename>\n\
\n\
List files that are matched with the <regex>.\n\
";
        return 1;
    }
    std::string pattern = argv[1];
    std::cout << "Pattern: '" << pattern << "'" << std::endl;
    try {
        list_files_regex(pattern);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
