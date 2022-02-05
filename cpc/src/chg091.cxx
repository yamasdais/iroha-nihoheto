#include <iostream>
#include <string>
#include <algorithm>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#include "challenge.h"

std::string get_hash(std::string const& password) {
    CryptoPP::SHA512 sha; 
    CryptoPP::byte digest[CryptoPP::SHA512::DIGESTSIZE];

    sha.CalculateDigest(digest, reinterpret_cast<CryptoPP::byte const*>(password.c_str()),
                        password.length());
    CryptoPP::HexEncoder encoder;
    std::string ret;
    encoder.Attach(new CryptoPP::StringSink(ret));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return ret;
}

void printPwhash() {
    auto hashed = get_hash("passabc012");
    std::cout << "hash:" << hashed << std::endl;;
    // 23B9451A3BA77C10C5E8DC5F141683E8964E234DDCBF9762E6CEFD5F588ACEBCBDE8B80F71E61CFBFB143DF57FC204DB52B284686368068821979B4ED9D6B259
}

void pwcheck() {
    std::string username, password;
    std::cout << "User: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    auto encPassword = get_hash(password);
    if (encPassword == "23B9451A3BA77C10C5E8DC5F141683E8964E234DDCBF9762E6CEFD5F588ACEBCBDE8B80F71E61CFBFB143DF57FC204DB52B284686368068821979B4ED9D6B259")
        std::cout << "Valid user" << std::endl;
    else
        std::cout << "Invalid password" << std::endl;

}

int main(int, char**) {
    //printPwhash();
    pwcheck();
    return 0;
}