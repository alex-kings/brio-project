#include "json/json.h"
#include <iostream>
#include "ReadJson.h"
#include "TrackGenerator.h"
#include "Piece.h"
#include <vector>
#include "Vec2D.h"

int main(int argc, char* argv[]) {

    std::cout << "Start" << "\n";

    // Get input from node program
    std::string input = argv[1];

    // User selection of pieces and their available quantities.
    Json::Value pieceSelection;
    try {
        // Example
        pieceSelection = readJson(input);
    }
    catch(const std::domain_error& e) {
        std::cerr << e.what() << std::endl;
    }

    // Get track
    Piece startPiece = getTrack(pieceSelection);
}






