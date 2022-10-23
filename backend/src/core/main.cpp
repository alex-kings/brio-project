#include "json/json.h"
#include <iostream>

int main(int argc, char* argv[]) {
    const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
    // Length of input
    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                       &err))
    {
        std::cout << "error" << std::endl;
        return EXIT_FAILURE;
    }
    
    const std::string name = root["Name"].asString();
    const int age = root["Age"].asInt();

    std::cout << name << std::endl;
    std::cout << age << std::endl;
    return EXIT_SUCCESS;
}