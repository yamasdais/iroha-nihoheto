#include <filesystem>

#include "challenge.h"

namespace fs = std::filesystem;
namespace chrono = std::chrono;

template <class Duration>
bool is_older_than(fs::path const& path, Duration const duration) {
    auto ftimeduration = fs::last_write_time(path).time_since_epoch();
    auto nowduration = (chrono::system_clock::now() - duration).time_since_epoch();
    return chrono::duration_cast<Duration>(nowduration - ftimeduration).count() > 0;
}

template <class Duration>
void print_files_older_than(fs::path const& path, Duration const duration) {
    try {
        if (fs::exists(path)) {
            if (is_older_than(path, duration)) {
                std::cout << path.native() << std::endl;
            }
        }
        if (fs::is_directory(path)) {
            for (auto const& entry : fs::directory_iterator(path)) {
                print_files_older_than(entry.path(), duration);
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main(int, char**) {
    using namespace std::chrono_literals;
    print_files_older_than(".", 1h + 20min);
    return 0;
}
