#include "json/json.h"
#include <iostream>
#include "Vec2D.h"
#include "Piece.h"
#include "ReadJson.h"


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

    Json::Value ressources = getPieceRessources(pieceSelection);

    std::cout << ressources << std::endl;

}