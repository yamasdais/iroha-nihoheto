#include <fstream>
#include <nlohmann/json.hpp>

#include "challenge.h"
#include "movie_data.h"

using json = nlohmann::json;

cpc::movie_list deserializeJSON(std::string const& filepath) {
    cpc::movie_list ret;
    std::ifstream ifile{filepath};
    if (ifile) {
        json jdat;
        try {
            ifile >> jdat;
            if (jdat.is_object()) {
                for (auto&& element : jdat.at("movies")) {
                    cpc::movie m{
                        element.at("id").get<unsigned int>(),
                        element.at("title").get<std::string>(),
                        element.at("year").get<int>(),
                        element.at("length").get<unsigned int>(),
                        {}, {}, {}
                    };
                    for (auto&& role : element.at("cast")) {
                        m.cast.emplace_back(cpc::casting_role{
                            role.at("star").get<std::string>(),
                            role.at("name").get<std::string>()
                        });
                    }
                    for (auto&& director : element.at("directors")) {
                        m.directors.push_back(director);
                    }
                    for (auto&& writer : element.at("writers")) {
                        m.writers.push_back(writer);
                    }

                    ret.push_back(std::move(m));
                }
            }
        } catch (std::exception const& ex) {
            std::cerr << ex.what() << std::endl;
        }
    }

    return ret;
}

void test0() {
    auto movies = deserializeJSON("../data/movies.json");

    for (auto const& m : movies) {
        std::cout << "Title: " << m.title << "\n";
    }

    std::cout << std::endl;
}

int main(int, char**) {
    test0();
    return 0;
}