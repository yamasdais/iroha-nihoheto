#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>

#include "challenge.h"
#include "movie_data.h"

using json = nlohmann::json;

namespace challenge100 {

void to_json(json& j, casting_role const& c) {
    j = json{{ "star", c.actor }, { "name", c.role }};
}

void to_json(json& j, movie const& m) {
    j = json::object({
        { "id", m.id },
        { "title", m.title },
        { "year", m.year },
        { "length", m.length },
        { "cast", m.cast },
        { "directors", m.directors },
        { "writers", m.writers }
    });
}

void serializeJSON(movie_list const& movies, std::string const& filepath) {
    json jdata{{ "movies", movies}};

    std::ofstream ofile(filepath.c_str());
    if (ofile) {
        ofile << std::setw(2) << jdata << std::endl;
    }
}

} // ns:challenge100

void test0() {
    auto movies = cpc::make_movie_list();

    cpc::serializeJSON(movies, "../data/movies.json");
}

int main(int, char**) {
    test0();
    return 0;
}