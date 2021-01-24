#include <fstream>
#include <string_view>
#include <filesystem>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "challenge.h"

    namespace fs = std::filesystem;
class logger {

    static constexpr inline std::string_view ext = ".tmp";
    fs::path logpath;
    static std::string make_filename() {
        boost::uuids::uuid uuid = boost::uuids::random_generator{}();
        return boost::uuids::to_string(uuid) + std::string{ext};
    }

    public:
    logger()
        : logpath{make_filename()}
    {
    }

    ~logger() {
        try {
            if (!logpath.empty())
                fs::remove(logpath);
        } catch (...) {
        }
    }

    void persist(fs::path const& path) {
        fs::rename(logpath, path);
        logpath.clear();
    }

    logger& operator<<(std::string_view message) {
        auto log = std::ofstream(logpath, std::ios::out | std::ios::app);
        log << message << "\n";
        return *this;
    }
};

int main(int, char**) {
    logger log;
    try {
        log << "log message" << "next line message";
        throw std::runtime_error("forced error");
    } catch (std::exception& e) {
        log << std::string{"caught exception: "} + e.what();
        log.persist(R"(chg038.log)");
    }
    return 0;
}
