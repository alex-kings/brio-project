#include <iostream>
#include <json/json.h>
#include <memory>


Json::Value getJson(std::string rawJson) {

    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                       &err)) {
        std::cout << "error" << std::endl;
        return NULL;
    }

    return root;
}

int main(int argc, char **argv) {
    std::cout << "starting";

    
}
