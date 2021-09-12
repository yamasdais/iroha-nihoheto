#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>

#include "challenge.h"
#include "movie_data.h"

using json = nlohmann::json;

void test0() {
    auto movies = cpc::make_movie_list();

    cpc::serializeJSON(movies, "../data/movies.json");
}

int main(int, char**) {
    test0();
    return 0;
}