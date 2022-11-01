#include "TrackGenerator.h"

Piece getTrack(const Json::Value& selection) {
    // Get vector of available pieces.
    std::vector<Piece> availablePieces = getAvailablePieces(selection);

    // Check if there is an available piece.
    if(availablePieces.size() < 1) {
        throw std::invalid_argument("Error: At least one piece should be provided.");
    }

    // Get first piece from available pieces.
    Piece firstPiece = availablePieces.back();
    availablePieces.pop_back();

    // Placed it in a vector of placed pieces.
    std::vector<Piece> placedPieces = {firstPiece};

    // Checks if the first piece has a free connector.
    

    // Connect pieces to the first piece in order to obtain a closed loop track.
    generateTrack(firstPiece, firstPiece.getConnectors()[0], &placedPieces, &availablePieces);
    
    return Piece();
}


bool generateTrack(const Piece& startPiece, const Connector& openConnector, std::vector<Piece>* placedPieces, std::vector<Piece>* availablePieces) {
    // Check size of available pieces
    if((*availablePieces).size() < 1) {
        return false;
    }

    // Iterate through all the available pieces to find one that can be placed next.
    Piece nextPiece = (*availablePieces).back();
    (*availablePieces).pop_back();
    

    return true;
}

std::vector<Piece> getAvailablePieces(const Json::Value& selection) {
    std::vector<Piece> availablePieces;
    Json::Value ressources = getPieceRessources(); // Library of pieces

    for(const std::string& member : selection.getMemberNames()) {
        for(int i = 0; i < std::stoi(selection[member].asString()); i++) {
            // Add Piece to available pieces.
            availablePieces.emplace_back(ressources[member]);
        }
    }

    return availablePieces;
}
