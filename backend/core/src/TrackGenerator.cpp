#include "TrackGenerator.h"

#include <iostream>

std::vector<Piece> generateTrack(const Json::Value& selection) {
    // Create empty track
    std::vector<Piece> track;

    // Pieces available
    Json::Value::Members availablePieces = selection.getMemberNames();

    // Print available pieces
    for(const Json::String& member : availablePieces ) {
        std::cout << member << "\n";
    }

    return track;
}