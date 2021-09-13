#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

#include <sqlite_modern_cpp.h>

#include "challenge.h"
#include "movie_data.h"

bool add_media(sqlite_int64 const movieid, std::string const& name, std::string const& description,
std::vector<char> content, sqlite::database& db) {
    try {
        db << R"( INSERT INTO media VALUES (?, ?, ?, ?) )"
           << movieid << name << description << content;

        return true;
    } catch (sqlite::sqlite_exception const& exc) {
        std::cerr << "Failed to insert media file: " << exc.what() << std::endl;
        return false;
    } catch (...) {
        return false;
    }
}

bool delete_media(sqlite_int64 const mediaid, sqlite::database& db) {
    try {
        db << R"( DELETE FROM media WHERE rowid = ? )"
           << mediaid;

        return true;
    } catch (sqlite::sqlite_exception const& exc) {
        std::cerr << "Failed to delete media file: " << exc.what() << std::endl;
        return false;
    } catch (...) {
        return false;
    }

}

cpc::media_list get_media(sqlite_int64 movieid, sqlite::database& db) {
    cpc::media_list ret;

    db << R"( SELECT rowid, * FROM media WHERE rowid = ?; )"
       << movieid
       >> [&ret](sqlite_int64 const rowid, sqlite_int64 const movieid, std::string const& name,
                 std::optional<std::string> const text, std::vector<char> const& blob) {
            ret.emplace_back(cpc::media{static_cast<u_int32_t>(rowid), static_cast<u_int32_t>(movieid),
                name, text, blob});
        };

    return ret;
}

std::vector<char> load_image(std::string_view filepath) {
    std::vector<char> ret;
    std::ifstream ifile(filepath.data(), std::ios::binary | std::ios::ate);

    if (ifile.is_open()) {
        auto size = ifile.tellg();
        ifile.seekg(0, std::ios::beg);

        ret.resize(static_cast<size_t>(size));
        ifile.read(reinterpret_cast<typename std::ifstream::char_type*>(ret.data()), size);
    }

    return ret;
}

void save_image(std::string_view filepath, std::vector<char> blob) {
    std::ofstream ofile(filepath.data(), std::ios::binary);
    if (ofile) {
        ofile.write(blob.data(), blob.size());
    }
}

void run_add(sqlite::database& db) {
    auto resource = load_image(R"(../data/the_matrix.jpg)");
    add_media(1, "the_matrix.jpg", "Main poster", resource, db);
}

void run_extract(sqlite::database& db) {
    auto media_list = get_media(1, db);
    uint32_t cnt = 0;
    std::string prefix{"../data/write_back"};
    for (auto const& media : media_list) {
        std::cout << "name: " << media.name << std::endl;
        save_image(prefix + std::to_string(cnt++) + ".jpg", media.blob);
    }
}

void run_list(sqlite::database& db) {
    db << R"(
SELECT T0.title, T1.name, T1.description
FROM movies T0
INNER JOIN media T1 ON T1.movieid = T0.rowid
          )"
        >> [](std::string const& title, std::string const& name, std::optional<std::string> description) {
           std::cout << title << ": " << name;
           if (description)
               std::cout << ", " << *description;
           std::cout << std::endl;
        };
}

void test0() {
    try {
        sqlite::database db{R"(../data/cpc086.db)"};
        //run_add(db);
        //run_extract(db);
        run_list(db);
    } catch (...) {
        std::cerr << "Error" << std::endl;
    }
}

int main(int, char**) {
    test0();

    return 0;
}