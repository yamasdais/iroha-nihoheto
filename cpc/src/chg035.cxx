#include <filesystem>
#include <numeric>

#include "challenge.h"

namespace fs = std::filesystem;
std::uintmax_t get_directory_size(fs::path const& dir, bool const follow_symlinks = false) {
    auto dir_it = fs::recursive_directory_iterator(
            dir, follow_symlinks
                ? fs::directory_options::follow_directory_symlink
                : fs::directory_options::none
            );

    return std::accumulate(fs::begin(dir_it), fs::end(dir_it), 0ull, [](
                std::uintmax_t total, fs::directory_entry const& entry) {
            return total + (fs::is_regular_file(entry) ?
                    fs::file_size(entry.path()) : 0ull);
            });
}

int main(int, char**) {
    std::string path;
    std::cout << "Path: ";
    std::cin >> path;
    std::cout << "Size: " << get_directory_size(path);
    return 0;
}

