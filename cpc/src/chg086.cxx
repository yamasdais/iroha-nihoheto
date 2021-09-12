
#include <iostream>
#include <string>

#include <sqlite_modern_cpp.h>

#include "challenge.h"
#include "movie_data.h"

sqlite_int64 get_person_id(std::string const& name, sqlite::database& db) {
    sqlite_int64 id = 0;
    db << R"(SELECT rowid FROM persons WHERE name = ?; )"
       << name
       >> [&id](sqlite_int64 const rowid) {
           id = rowid;
       };

    return id;
}

sqlite_int64 insert_person(std::string const& name, sqlite::database& db) {
    db << R"( INSERT INTO persons VALUES(?); )"
       << name;
    return db.last_insert_rowid();
}

void insert_directors(sqlite_int64 movie_id, std::vector<std::string> const& directors,
    sqlite::database& db)
{
    for (auto const& director : directors) {
        auto id = get_person_id(director, db);
        if (id == 0) {
            id = insert_person(director, db);
        }

        db << R"( INSERT INTO directors VALUES(?, ?); )"
           << movie_id << id;
    }
}

void insert_writers(sqlite_int64 movie_id, std::vector<std::string> const& writers,
sqlite::database& db) {
    for (auto const& writer : writers) {
        auto id = get_person_id(writer, db);
        if (id == 0) {
            id = insert_person(writer, db);
        }
        db << R"( INSERT INTO writers VALUES(?, ?); )"
           << movie_id << id;
    }
}

void insert_casts(sqlite_int64 movie_id, std::vector<cpc::casting_role> const& casts,
sqlite::database& db) {
    for (auto const& cr : casts) {
        auto id = get_person_id(cr.actor, db);
        if (id == 0) {
            id = insert_person(cr.actor, db);
        }

        db << R"( INSERT INTO casting VALUES(?, ?, ?); )"
           << movie_id << id << cr.role;
    }
}

void insert_movie(cpc::movie& m, sqlite::database& db) {
    try {
        db << "begin;";
        db << R"( INSERT INTO movies VALUES(?, ?, ?); )"
           << m.title << m.year << m.length;

        auto movie_id = db.last_insert_rowid();
        insert_directors(movie_id, m.directors, db);
        insert_writers(movie_id, m.writers, db);
        insert_casts(movie_id, m.cast, db);

        m.id = static_cast<unsigned int>(movie_id);

        db << R"(commit;)";
    } catch (std::exception& exc) {
        db << "rollback;";
        std::cerr << "error: " << exc.what() << std::endl;
    }

}

void test0() {
    auto inputMovies = cpc::deserializeJSON("../data/appendMovies.json");
    auto db = sqlite::database{R"(../data/tmp.db)"};

    for (auto m : inputMovies) {
        insert_movie(m, db);
    }
}

int main(int, char**) {
    test0();
    return 0;
}