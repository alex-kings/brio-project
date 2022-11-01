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

    // Connect pieces to the first piece in order to obtain a closed loop track.
    generateTrack(firstPiece, firstPiece.getConnector(0), &placedPieces, &availablePieces);
    
    return Piece();
}



bool generateTrack(const Piece& startPiece, const Connector& openConnector, std::vector<Piece>* placedPieces, std::vector<Piece>* availablePieces) {
    // Check size of available pieces
    if((*availablePieces).size() < 1) {
        return false;
    }

    // Iterate through all the available pieces to find one that can be placed next.
    for(uint i = 0; i < availablePieces->size(); i++) {
        Piece& testPiece = (*availablePieces)[i];

        // Finds is the test piece has a connector of the opposite type to the open one.
        for(uint j = 0; j < testPiece.getNumberConnectors(); j++) {
            Connector& testCon = testPiece.getConnector(j);

            // Break out of iteration if the connector types are the same 
            if(testCon.getType() == openConnector.getType()) continue;
            
            // Get angle between the open connector and the test connector.
            float angle = openConnector.getDirection().getAngleDifference(testCon.getDirection());

            // Rotates the test piece so that the connectors are aligned

            // Get position difference between the two connectors

            // Translates the test piece so that the connectors are at the same position


            // Test collision between the test piece and all previously placed pieces

        }
    }

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
