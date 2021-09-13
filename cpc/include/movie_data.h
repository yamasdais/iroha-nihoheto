#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <nlohmann/json.hpp>

namespace challenge100 {
struct casting_role {
    std::string actor;
    std::string role;
};

struct movie {
    unsigned int id;
    std::string title;
    int year;
    unsigned int length;

    std::vector<casting_role> cast;
    std::vector<std::string> directors;
    std::vector<std::string> writers;

    void print() const {
        std::cout << id << ", " << title << ", " << year << ", " << length << "min" << "\n";
        std::cout << "actors: " << "\n";
        for (auto const& c : cast) {
            std::cout << c.role << ": " << c.actor << "\n";
        }
        std::cout << "directors: ";
        for (auto const& d : directors) {
            std::cout << d << ",";
        }
        std::cout << "\nwriters: ";
        for (auto const& w : writers) {
            std::cout << w << ",";
        }
        std::cout << std::endl;
    }
};

struct media {
    uint32_t id;
    uint32_t movie_id;
    std::string name;
    std::optional<std::string> description;
    std::vector<char> blob;
};

using movie_list = std::vector<movie>;
using media_list = std::vector<media>;

movie_list const make_movie_list() {
    return movie_list{
        {
            11001, "The Matrix", 1999, 196,
            {
                { "Keanu Reeves", "Neo" },
                { "Laurence Fishburne", "Morpheus" },
                { "Carrie-Anne Moss", "Trinity" },
                { "Hugo Weaving", "Agent Smith" }
            },
            { "Lana Wachowski", "Lilly Wachowski" },
            { "Lana Wachowski", "Lilly Wachowski" },
        }, {
            9871, "Forrest Gump", 1994, 202,
            {
                { "Tom Hanks", "Forrest Gump" },
                { "Sally Field", "Mrs. Gump" },
                { "Robin Wright", "Jenny Curran" },
                { "Mykelti Williamson", "Bubba Blue" },
            },
            { "Robert Zemeckis" },
            { "inston Groom", "Eric Roth" },
        },
    };
}

void to_json(nlohmann::json& j, casting_role const& c) {
    j = nlohmann::json{{ "star", c.actor }, { "name", c.role }};
}

void to_json(nlohmann::json& j, movie const& m) {
    j = nlohmann::json::object({
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
    nlohmann::json jdata{{ "movies", movies}};

    std::ofstream ofile(filepath.c_str());
    if (ofile) {
        ofile << std::setw(2) << jdata << std::endl;
    }
}

movie_list deserializeJSON(std::string const& filepath) {
    cpc::movie_list ret;
    std::ifstream ifile{filepath};
    if (ifile) {
        nlohmann::json jdat;
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

}