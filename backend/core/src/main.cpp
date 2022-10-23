#include "json/json.h"
#include <iostream>
#include "Vec2D.h"


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
        // Throw an exception
        throw std::domain_error("Error in parsing JSON input: String is not in correct JSON format.");
    }

    return root;
}

int main(int argc, char* argv[]) {
    // Get piece selection in JSON format from argv.
    //std::cout << argv[1] << std::endl;


    try {
        //Json::Value pieceSelection = readJson(argv[1]);

        // Example
        Json::Value pieceSelection = readJson("{\"A\":\"10\", \"B\":\"5\", \"E\":\"9\"}");
    }
    catch(const std::domain_error& e) {
        std::cerr << e.what() << std::endl;
    }
    

    std::cout << "Test" << std::endl;

    Vec2D v(3.2, 4.3);
}