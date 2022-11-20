#include "TrackGenerator.h"

Piece getTrack(const Json::Value& selection) {
    // Get vector of available pieces.
    std::vector<Piece> pieces = getAvailablePieces(selection);

    // Check if there is an available piece.
    if(pieces.size() < 1) {
        throw std::invalid_argument("Error: At least one piece should be provided.");
    }

    // TEST rotate the first piece around by pi/2
    pieces[0].rotate(pieces[0].getConnector(0).getPosition(), 3.14/2);

    // TEST write result to file
    writeTrackToFile(pieces);

    // Get first piece from available pieces.
    Piece& firstPiece = pieces.back();

    // Mark this piece as used.
    firstPiece.setUsed(true);

    // Connect pieces to the first piece in order to obtain a closed loop track.
    generateTrack(firstPiece, firstPiece.getConnector(0), &pieces);
    
    return Piece();
}


bool generateTrack(const Piece& startPiece, const Connector& openConnector, std::vector<Piece>* pieces) {
    // Check size of available pieces
    if(pieces->size() < 1) {
        return false;
    }

    // Try to find a piece that can be placed
    for(Piece& testPiece : (*pieces)) {

        // Finds if the test piece has a connector of the opposite type to the open one.
        for(uint j = 0; j < testPiece.getNumberConnectors(); j++) {
            Connector& testCon = testPiece.getConnector(j);

            // Break out of iteration if the connector types are the same 
            if(testCon.getType() == openConnector.getType()) continue;
            
            // Get angle between the open connector and the test connector.
            float angleDiff = openConnector.getDirection().getAngleDifference(testCon.getDirection());

            // Rotate the test piece around its right connector to align with the open connector
            testPiece.rotate(testCon.getPosition(), M_PI - angleDiff);

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


void writeTrackToFile(const std::vector<Piece>& track) {


    // Write result to the result file
    std::ofstream file("../tracks/track_result.json");

    file << "{\"pieces\":[";

    for(uint i = 0; i < track.size(); i++) {
        file << track[i].toJson();
        if(i < track.size() - 1) file << ",";
    }

    file << "]}";

    file.close();
    std::cout << "done" << "\n";
}
