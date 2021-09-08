#include <iostream>
#include <string>

#include <sqlite_modern_cpp.h>

#include "challenge.h"
#include "movie_data.h"

std::vector<cpc::casting_role> get_casts(sqlite::database& db, sqlite_int64 const movie_id) {
    std::vector<cpc::casting_role> ret;
    db << R"(SELECT p.name, c.role FROM casting AS c
             INNER JOIN persons AS p ON c.personid = p.rowid
             WHERE c.movieid = ?
            ;)"
       << movie_id
       >> [&ret](std::string const name, std::string const role) {
           ret.emplace_back(cpc::casting_role{ name, role });
       };
    return ret;
}

std::vector<std::string> get_directors(sqlite::database& db, sqlite_int64 const movie_id) {
    std::vector<std::string> ret;
    db << R"(SELECT p.name FROM directors AS d
             INNER JOIN persons AS p ON d.personid = p.rowid
             WHERE d.movieid = ?
            ;)"
       << movie_id
       >> [&ret](std::string const name) {
           ret.emplace_back(name);
       };

    return ret;
}

std::vector<std::string> get_writers(sqlite::database& db, sqlite_int64 const movie_id) {
    std::vector<std::string> ret;
    db << R"(SELECT p.name FROM writers AS w
             INNER JOIN persons AS p ON w.personid = p.rowid
             WHERE w.movieid = ?
            ;)"
       << movie_id
       >> [&ret](std::string const name) {
           ret.emplace_back(name);
       };

    return ret;
}

cpc::movie_list get_movies(sqlite::database& db) {
    cpc::movie_list ret;
    db << R"(select rowid, * from movies)"
       >> [&ret, &db](sqlite3_int64 const rowid, std::string const& title, int const year, int const length) {
           ret.emplace_back(cpc::movie{
               static_cast<uint32_t>(rowid), title, year, static_cast<uint32_t>(length),
               get_casts(db, rowid), get_directors(db, rowid), get_writers(db, rowid)
           });

       };
    return ret;
}

void test0() {
    try {
        sqlite::database db(R"(../data/cpc085.db)");
        auto movies = get_movies(db);
        for (auto movie : movies) {
            movie.print();
        }
    } catch (sqlite::sqlite_exception const& exc) {
        std::cerr << exc.get_code() << ": " << exc.what() << " during"
                  << exc.get_sql() << std::endl;
    } catch (std::exception const& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

int main(int, char**) {
    test0();
    return 0;
}