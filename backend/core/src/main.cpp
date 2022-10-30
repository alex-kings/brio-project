#include "json/json.h"
#include <iostream>
#include "ReadJson.h"
#include "TrackGenerator.h"
#include "Piece.h"
#include <vector>
#include "Vec2D.h"


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