#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <codecvt>
#include <vector>
#include <map>
#include <algorithm>
#include <locale>
#include <optional>

#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>
#include <curlcpp/curl_easy.h>
#include <curlcpp/curl_exception.h>
#include <curlcpp/curl_form.h>
#include <curlcpp/curl_info.h>

using json = nlohmann::json;

std::wstring utf8_to_utf16(std::string_view text)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wtext = converter.from_bytes(text.data());
    return wtext;
}

std::string utf16_to_utf8(std::wstring_view wtext)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string text = converter.to_bytes(wtext.data());
    return text;
}

struct Translator {
    Translator(std::string_view endpoint, std::string_view key)
    : endpoint{endpoint}
    , key{key}
    {}

    auto makeForm(std::string_view targetLang, std::string_view text) {
        using pair = curl::curl_pair<CURLformoption, std::string>;
        pair auth_n{CURLFORM_COPYNAME, "auth_key"};
        pair auth_c{CURLFORM_COPYCONTENTS, key};
        pair target_n{CURLFORM_COPYNAME, "target_lang"};
        pair target_c{CURLFORM_COPYCONTENTS, targetLang.data()};
        pair text_n{CURLFORM_COPYNAME, "text"};
        pair text_c{CURLFORM_COPYCONTENTS, text.data()};

        curl::curl_form form;
        form.add(auth_n, auth_c);
        form.add(target_n, target_c);
        form.add(text_n, text_c);

        return form;
    }

    auto translate(std::string_view targetLang, std::string_view text) {
        std::stringstream str;
        curl::curl_ios<std::stringstream> writer{str};
        curl::curl_easy easy{writer};

        try {
            //auto urlencKey = boost::algorithm::replace_all_copy(key, ":", "%58");
            auto host = endpoint;
            easy.add<CURLOPT_URL>(host.c_str());
            easy.add<CURLOPT_SSL_VERIFYPEER>(false);
            easy.add<CURLOPT_HTTPPOST>(makeForm(targetLang, text).get());

            easy.perform();
            auto info = easy.get_info<CURLINFO_CONTENT_TYPE>();
            std::cout << info.get() << std::endl;

            std::cout << str.str() << std::endl;
        } catch (curl::curl_easy_exception const& ex) {
            std::cerr << ex.what() << std::endl;
            ex.print_traceback();
        }
    }
  private:
    std::string endpoint;
    std::string key;
};

std::optional<std::string> setupKey() {
    auto const keyPath = "../data/deepL.key";
    std::ifstream istr{keyPath};

    if (istr) {
        std::string firstLine;
        istr >> firstLine;
        boost::trim(firstLine);
        return {firstLine};
    } else {
        std::cerr << "cannot open file: " << keyPath << std::endl;
    }

    return {};
}

void checkTranslate() {
    auto apiKey = setupKey();
    if (apiKey) {
        Translator translator{"https://api-free.deepl.com/v2/translate", apiKey.value()};
        translator.translate("JA", "Hello, World!");
    }
}

int main(int, char**) {
    checkTranslate();
    return 0;
}