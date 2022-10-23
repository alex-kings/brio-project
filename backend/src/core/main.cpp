#include "json/json.h"
#include <iostream>

/**
 * Return the Json object representation of a string input.
*/
Json::Value readJson(std::string rawJson) {
    // Get length of input
    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
        std::cout << "Error in parsing JSON input." << std::endl;
        return NULL;
    }

    return root;
}

int main(int argc, char* argv[]) {
    // Get piece selection in JSON format from argv.

    std::cout << argv[1] << std::endl;

    Json::Value pieceSelection = readJson(argv[1]);

    std::cout << pieceSelection["A"] << std::endl;
}