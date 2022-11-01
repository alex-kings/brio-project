#include "ReadJson.h"
#include <iostream>

Json::Value readJson(const std::string& rawJson) {
    // Get length of input
    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
        // Throw an exception
        throw std::domain_error("Error in parsing JSON input: String is not in correct JSON format.");
    }

    return root;
}

Json::Value getPieceRessources() {
    // Read pieces.json file.
    std::ifstream piecesFile("../ressources/pieces.json");
    std::stringstream buffer;
    buffer << piecesFile.rdbuf();

    // Parse to Json object.
    return readJson(buffer.str());
}

