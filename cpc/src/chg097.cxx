#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <algorithm>

#include <nlohmann/json.hpp>
#include <curlcpp/curl_easy.h>
#include <curlcpp/curl_exception.h>

using json = nlohmann::json;

struct exchange_info {
    double delay_15m_price;
    double latest_price;
    double buying_price;
    double selling_price;
    std::string symbol;
};

using blockchain_rates = std::map<std::string, exchange_info>;

void from_json(json const& jdata, exchange_info& info) {
    info.delay_15m_price = jdata.at("15m").get<double>();
    info.latest_price = jdata.at("last").get<double>();
    info.buying_price = jdata.at("buy").get<double>();
    info.selling_price = jdata.at("sell").get<double>();
    info.symbol = jdata.at("symbol").get<std::string>();
}

std::stringstream get_json_document(std::string_view url) {
    std::stringstream ret;

    try {
        curl::curl_ios<std::stringstream> writer{ret};
        curl::curl_easy easy{writer};

        easy.add<CURLOPT_URL>(url.data());
        easy.add<CURLOPT_FOLLOWLOCATION>(1L);

        easy.perform();
    } catch (curl::curl_easy_exception const& error) {
        auto errors = error.get_traceback();
        error.print_traceback();
    }

    return ret;
}

void fetchInfo() {
    auto doc = get_json_document("https://blockchain.info/ticker");

    json jdata;
    doc >> jdata;

    blockchain_rates rates = jdata;

    for (auto const& kv : rates) {
        std::cout << "1BPI = " << kv.second.latest_price
            << " " << kv.first << std::endl;
    }
}

int main(int, char**) {
    fetchInfo();
    return 0;
}