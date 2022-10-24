#include "json/json.h"
#include <iostream>
#include "Vec2D.h"
#include "Piece.h"
#include <fstream>


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


/**
 * Get collection of pieces and their available quantities from the user selection.
 * This may throw a domain error exception.
*/
std::vector<std::pair<Piece, int>> getPiecesAvailable(Json::Value piecesJson) {
    // Read pieces.json file.
    std::ifstream piecesFile("../ressources/pieces.json");
    std::stringstream buffer;
    buffer << piecesFile.rdbuf();

    // Parse to Json object.
    Json::Value piecesRessource;
    std::string str = buffer.str();
    
    piecesRessource = readJson(str);

}


int main(int argc, char* argv[]) {
    std::cout << "Start" << std::endl;

    // User selection of pieces and their available quantities.
    Json::Value pieceSelection;
    try {
        // Example
        pieceSelection = readJson("{\"A\":\"10\", \"B\":\"5\", \"E\":\"9\"}");
    }
    catch(const std::domain_error& e) {
        std::cerr << e.what() << std::endl;
    }

    

    

}