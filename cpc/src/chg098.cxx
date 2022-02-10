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

class imap_connection {
    void setup_easy(curl::curl_easy& easy) {
        easy.add<CURLOPT_PORT>(port);
        easy.add<CURLOPT_USERNAME>(user.c_str());
        easy.add<CURLOPT_PASSWORD>(pass.c_str());
        easy.add<CURLOPT_USE_SSL>(CURLUSESSL_ALL);
        easy.add<CURLOPT_SSL_VERIFYPEER>(0L);
        easy.add<CURLOPT_SSL_VERIFYHOST>(0L);
        easy.add<CURLOPT_USERAGENT>("libcurl-agent/1.0");
    }

    std::string const url;
    unsigned short const port;
    std::string const user;
    std::string const pass;

  public:
    imap_connection(
        std::string const& url
        , unsigned short const port
        , std::string_view user
        , std::string_view pass
    ): url{url}
    , port{port}
    , user{user}
    , pass{pass}
    {}

    std::string get_folders() {
        std::stringstream str;

        try {
            curl::curl_ios<std::stringstream> writer(str);
            curl::curl_easy easy{writer};

            easy.add<CURLOPT_URL>(url.c_str());
            setup_easy(easy);

            easy.perform();
        } catch (curl::curl_easy_exception const& ex) {
            ex.print_traceback();
        }

        return str.str();
    }

    std::vector<unsigned int> fetch_unread_uids(std::string const& folder) {
        std::stringstream str;
        try {
            curl::curl_ios<std::stringstream> writer(str);
            curl::curl_easy easy{writer};

            easy.add<CURLOPT_URL>((url + "/" + folder + "/").c_str());
            easy.add<CURLOPT_CUSTOMREQUEST>("SEARCH UNSEEN");
            setup_easy(easy);

            easy.perform();
        } catch (curl::curl_easy_exception const& ex) {
            ex.print_traceback();
        }
        std::vector<unsigned int> uids;
        str.seekg(8, std::ios::beg);
        unsigned int uid;
        while (str >> uid) {
            uids.push_back(uid);
        }

        return uids;
    }

    std::string fetch_email(std::string_view folder, unsigned int uid) {
        std::stringstream str;
        try {
            curl::curl_ios<std::stringstream> writer(str);
            curl::curl_easy easy{writer};
            easy.add<CURLOPT_URL>((url + "/" + folder.data() + "/;UID=" + std::to_string(uid)).c_str());

            setup_easy(easy);

            easy.perform();
        } catch (curl::curl_easy_exception const& ex) {
            ex.print_traceback();
        }
        return str.str();
    }
};

void fetchMail() {
    std::string user;
    std::string pass;
    std::cout << "User? ";
    std::cin >> user;
    std::cout << "Pass? ";
    std::cin >> pass;
    imap_connection imap("imaps://imap.nifty.com", 993, user, pass);
    auto folders = imap.get_folders();
    std::cout << folders << std::endl;

    auto uids = imap.fetch_unread_uids("inbox");
    if (!uids.empty()) {
        auto email = imap.fetch_email("inbox", uids.back());
        std::cout << email << std::endl;
    }
}
int main(int, char**) {
    fetchMail();
    return 0;
}