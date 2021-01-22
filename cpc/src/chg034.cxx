#include <fstream>
#include <filesystem>
#include <boost/filesystem.hpp>

#include "challenge.h"

//namespace fs = std::filesystem;
namespace boostfs = boost::filesystem;

/*
 * temporary ファイル作成時、tempnam() はあまり好ましくない。
 * mkstemp() <unistd.h> は、c++ から使いづらい。
 * std::filesystem は temp ディレクトリ名を取得できるが、tempファイル名を作る関数はない。
 * 結局 boost::filesystem を使うことにした。
 */
void remove_empty_lines(boostfs::path filepath) {
    std::ifstream infile{filepath.native(), std::ios::in};
    if (!infile.is_open())
        throw std::runtime_error("cannot open file: " + filepath.native());

    auto temppath = boostfs::temp_directory_path() / boostfs::unique_path();
    std::cout << "temp: " << temppath.native() << std::endl;;

    std::ofstream outfile{temppath.native(), std::ios::out | std::ios::trunc};
    std::string line;
    while (std::getline(infile, line)) {
        if (line.length() > 0 && line.find_first_not_of(' ') != line.npos)
            outfile << line << '\n';
    }

    infile.close();
    outfile.close();

    
    boostfs::remove(temppath);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: chg034 <input file>\n\
\n\
Removes empty lines in <input file>.\n\
";
        return 1;
    }
    remove_empty_lines(std::string(argv[1]));
    return 0;
}
