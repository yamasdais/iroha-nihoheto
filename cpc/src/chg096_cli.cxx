#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <algorithm>

#define ASIO_STANDALONE
#include <boost/asio.hpp>

namespace asio = boost::asio;

void run_client(std::string const& host, uint16_t port) {
    try {
        asio::io_context context;
        asio::ip::tcp::socket tcp_socket(context);
        asio::ip::tcp::resolver resolver(context);
        asio::connect(tcp_socket, resolver.resolve({host.c_str(), std::to_string(port)}));

        while (true) {
            std::cout << "number [1-99]: ";
            int number;
            std::cin >> number;

            if (std::cin.fail() || number < 1 || number > 99)
                break;

            auto request = std::to_string(number);
            tcp_socket.write_some(asio::buffer(request, request.length()));

            std::array<char, 1024> reply;
            auto replyLength = tcp_socket.read_some(asio::buffer(reply, reply.size()));

            std::cout << "replied: ";
            std::cout.write(reply.data(), replyLength);
            std::cout << std::endl;
        }

    } catch (std::exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }
}

int main(int, char**) {
    run_client("localhost", 11234);
    return 0;
}