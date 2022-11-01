#include "TrackGenerator.h"
#include <iostream>
#include <stdexcept>

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

    // Connect pieces to the first piece in order to obtain a closed loop track.
    generateTrack(firstPiece, firstPiece, placedPieces, availablePieces);
    return Piece();
}


bool generateTrack(const Piece& startPiece, const Piece& lastPiece, const std::vector<Piece>& placedPieces, const std::vector<Piece>& availablePieces) {
    
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
