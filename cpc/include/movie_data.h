#pragma once

#include <string>
#include <vector>
#include <iostream>

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

using movie_list = std::vector<movie>;

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

}