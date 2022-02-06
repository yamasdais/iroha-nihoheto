#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

#include "challenge.h"

namespace fs = std::filesystem;

template <class HashType>
std::string compute_hash(fs::path const& filepath) {
    std::string digest;
    HashType hash;

    // new したオブジェクトを直接渡しているので、メモリリークが心配になるが、
    // Crypto++ は所有権を呼び出し先に委譲する設計が多いのでこれで良い。
    // https://www.cryptopp.com/wiki/FileSource
    CryptoPP::FileSource src(
        filepath.c_str(), true, new CryptoPP::HashFilter(hash,
                    new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest)))
    );

    return digest;
}

void printHash() {
    std::string path;
    std::cout << "Path? ";
    std::cin >> path;

    try {
        std::cout << "SHA1: "
                  << compute_hash<CryptoPP::SHA1>(path) << std::endl;
        std::cout << "SHA256: "
                  << compute_hash<CryptoPP::SHA256>(path) << std::endl;
        std::cout << "MD5: "
                  << compute_hash<CryptoPP::Weak::MD5>(path) << std::endl;
    } catch (std::exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }
}

int main(int, char**) {
    printHash();
    return 0;
}