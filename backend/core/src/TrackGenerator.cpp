#include "TrackGenerator.h"

Piece getTrack(const Json::Value& selection) {
    // Get vector of available pieces.
    std::vector<Piece> pieces = getAvailablePieces(selection);

    // Check if there is an available piece.
    if(pieces.size() < 1) {
        throw std::invalid_argument("Error: At least one piece should be provided.");
    }


    // Get first piece from available pieces.
    Piece& firstPiece = pieces.back();

    // Mark this piece as used.
    firstPiece.setUsed(true);

    // Connect pieces to the first piece in order to obtain a closed loop track.
    generateTrack(firstPiece, firstPiece.getConnector(0), &pieces);


    // TEST write result to file
    writeTrackToFile(pieces);

    
    return Piece();
}


bool generateTrack(const Piece& startPiece, const Piece& lastPiece, Connector& openConnector, std::vector<Piece>* pieces) {
    // Check size of available pieces
    if(pieces->size() < 1) {
        return false;
    }

    // Look for pieces that can be placed
    for(Piece& testPiece : (*pieces)) {

        if(testPiece.isUsed()) continue; // Skip pieces already placed

        // Finds if the test piece has a connector of the opposite type to the open one.
        for(uint j = 0; j < testPiece.getNumberConnectors(); j++) {
            Connector& testCon = testPiece.getConnector(j);

            if(!testCon.isFree()) continue; // The connector is not free.

            if(testCon.getType() == openConnector.getType()) continue; // Skip connectors of the same type.

            // Angle and position difference between the two connectors.            
            float angleDiff = openConnector.getDirection().getAngleDifference(testCon.getDirection());
            Vec2D positionDiff = openConnector.getPosition() - testCon.getPosition();

            // Connects the pieces around their two connectors.
            testPiece.rotate(testCon.getPosition(), M_PI - angleDiff);
            testPiece.translate(positionDiff);


            // Checks whether the test piece collides with any placed piece.
            bool noCollision = true;
            for(const Piece& testCollisionPiece : (*pieces)) {
                if(!testCollisionPiece.isUsed()) continue; // Skip unplaced pieces.

                // Assume that two consecutive pieces can never collide with each other.
                if(&testCollisionPiece == &lastPiece) continue;

                if (testPiece.collides(testCollisionPiece)) {
                    noCollision = false;
                    break; // Test piece can not be placed.
                }
            }

            if(noCollision) {
                // THIS PIECE IS PLACEABLE
                // Place this piece!

                testPiece.setUsed(true);

                // Link the two connectors together
                testCon.link(&openConnector);

                // Checks whether this piece has another free connector
                if(!testPiece.hasOpenConnectors()) return true; // This piece has no more available connectors. The track is built.

                Connector openCon = testPiece.getOpenConnector(); // Get the open connector
                
                // Place the next piece.
                if( generateTrack(startPiece, testPiece, openCon, pieces) ) {
                    // The track could be built! return true.
                    return true;
                }
                else {
                    // The track could not be build. Unlink and unplace pieces.
                    testPiece.setUsed(false);
                    testCon.unlink(&openConnector);

                    // Re-attempt with next placeable piece.
                }
            }
        }
    }
    // No track was found for any placeable piece.
    return false;
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
