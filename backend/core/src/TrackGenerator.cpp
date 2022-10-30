#include "TrackGenerator.h"
#include <unordered_map>
#include <iostream>

Piece getTrack(const Json::Value& selection) {
    std::unordered_map<std::string,int>selectionMap;
    // Casts the selection as a map
    for(const std::string& member : selection.getMemberNames()) {
        selectionMap[member] = std::stoi(selection[member].asString());
    }

    return Piece();
}


bool generateTrack(const Piece& startPiece, const Piece& lastPiece, std::map<Piece,int> selection, std::vector<Piece> placedPieces) {
    
    return true;
}