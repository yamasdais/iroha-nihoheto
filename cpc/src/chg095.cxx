#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define ASIO_STANDALONE
#include <boost/asio.hpp>

namespace asio = boost::asio;

std::vector<std::string> get_ip_address(std::string const& hostname) {
    std::vector<std::string> ret;

    try {
        asio::io_context context;
        asio::ip::tcp::resolver resolver{context};
        auto endpoints = resolver.resolve(asio::ip::tcp::v4(), hostname.c_str(), "");

        for (auto const& e : endpoints) {
            ret.emplace_back(e.endpoint().address().to_string());
        }
    } catch (std::exception const& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return ret;
}

void printHostAddress() {
    constexpr auto hostname = "www.google.com";
    auto ips = get_ip_address(hostname);
    std::cout << "Hostname: " << hostname << "\n";
    for (auto const& ip : ips) {
        std::cout << ip << std::endl;
    }
}

int main(int, char**) {
    printHostAddress();

    return 0;
}