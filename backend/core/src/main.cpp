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

    // Get track
    // Piece startPiece = getTrack(pieceSelection);

    // DEBUG

    // Get library of pieces
    Json::Value res = getPieceRessources();
    Piece piece = Piece(res["A"]);
    piece.getConnectors()[0].link(&piece.getConnectors()[1]);


    std::cout << "Conn are equal ?" << (&piece.getConnectors()[1] == piece.getConnectors()[0].getConnection()) << std::endl;


    if(piece.getConnectors()[1].isFree()) {
        std::cout << "Connector free." << "\n";
    }
    else {
        std::cout << "Not free." << "\n";
    }
}
