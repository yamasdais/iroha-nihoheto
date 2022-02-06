#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/default.h>

namespace fs = std::filesystem;

void encrypt_file(
    fs::path const& srcPath,
    fs::path const& dstPath,
    std::string const& key
) {
    CryptoPP::FileSource src{
        srcPath.c_str(), true,
        new CryptoPP::DefaultEncryptorWithMAC{
            (CryptoPP::byte*)key.c_str(), key.size(),
            new CryptoPP::FileSink(dstPath.c_str())
        }
    };
}

void encrypt_file(
    fs::path const& filepath,
    std::string const& key
) {
    auto tempPath = fs::temp_directory_path() / filepath.filename();
    encrypt_file(filepath, tempPath, key);

    fs::remove(filepath);
    fs::rename(tempPath, filepath);
}

void decrypt_file(
    fs::path const& srcPath,
    fs::path const& dstPath,
    std::string const& key
) {
    CryptoPP::FileSource src{
        srcPath.c_str(), true,
        new CryptoPP::DefaultDecryptorWithMAC{
            (CryptoPP::byte*)key.c_str(), key.size(),
            new CryptoPP::FileSink{ dstPath.c_str() }
        }
    };
}

void decrypt_file(
    fs::path const& filePath,
    std::string const& key
) {
    auto tempPath = fs::temp_directory_path() / filePath.filename();
    decrypt_file(filePath, tempPath, key);

    fs::remove(filePath);
    fs::rename(tempPath, filePath);
}

void checkEncDec() {
    constexpr auto const keyword = "cppchallenge100";
    constexpr auto const encFile = "sample.txt.enc";
    constexpr auto const decFile = "sample.txt.dec";
    encrypt_file("../data/sample.txt", encFile, keyword);
    decrypt_file(encFile, decFile, keyword);

}

int main(int, char**) {
    checkEncDec();
    return 0;
}