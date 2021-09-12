#include <fstream>
#include <nlohmann/json.hpp>

#include "challenge.h"
#include "movie_data.h"

using json = nlohmann::json;

void test0() {
    auto movies = cpc::deserializeJSON("../data/movies.json");

    for (auto const& m : movies) {
        std::cout << "Title: " << m.title << "\n";
    }

    std::cout << std::endl;
}

int main(int, char**) {
    test0();
    return 0;
}