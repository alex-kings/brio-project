#include "TrackGenerator.h"
#include <iostream>

Piece getTrack(const Json::Value& selection) {
    std::vector<Piece> availablePieces = getAvailablePieces(selection);
    
    return Piece();
}


bool generateTrack(const Piece& startPiece, const Piece& lastPiece, std::map<Piece,int> selection, std::vector<Piece> placedPieces) {
    
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
