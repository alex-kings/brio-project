#include "Track.h"
#include "ParseJson.h"
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iostream>

Track::Track(const Json::Value& selection) {
    Json::Value ressources = getPieceRessources(); // Library of pieces

    for(const std::string& member : selection.getMemberNames()) {
        for(int i = 0; i < std::stoi(selection[member].asString()); i++) {
            // Add Piece to available pieces.
            pieces.emplace_back(ressources[member]);
        }
    }

    // Validation conditions
    validationAngle =  2*0.31415; // ~2*18 degrees.
    validationDist = pieces.size() * 5;
    minPieceNb = std::floor(pieces.size()*0.6); // 60% of pieces

    // OPTIONAL: Shuffles the pieces before starting generation
    // obtain a time-based seed:
    // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // std::default_random_engine randomEngine = std::default_random_engine(seed);
    std::shuffle(pieces.begin(), pieces.end(), randomEngine);

    // Get first piece from available pieces.
    firstPiece = &pieces.back();
    validationConnector = &firstPiece->getConnector(1);

    // Mark this piece as used.
    firstPiece->setUsed(true);

    // Generate the track!
    bool res = generateTrack((*firstPiece), firstPiece->getConnector(0));
    std::cout << "Track could be generated: " << std::to_string(res) << "\n";

    // Write result to file
    writeToFile();

    std::cout << "Recursions: " << std::to_string(count) << "\n";

}


bool Track::generateTrack(const Piece& lastPiece, Connector& openConnector) {
    count ++;

    // std::cout << "Round number: " << std::to_string(count) << "\n";

    // Keep track of previously tested pieces
    std::unordered_set<std::string> previouslyTested;

    // Look for pieces that can be placed
    // for(Piece& testPiece : pieces) {

    //     if(testPiece.isUsed()) continue; // Skip pieces already placed

    //     // Checks whether the piece has already been tested
    //     if(previouslyTested.count(testPiece.getId())) continue;
    //     previouslyTested.insert(testPiece.getId());

    //     // Attempt placement of this piece.
    //     if(attemptPlacement(testPiece, lastPiece, openConnector)) return true;

    //     if(testPiece.isFlippable()) {
    //         testPiece.flip();
    //     }

    //     // Re-attempt placement of this piece after flipping.
    //     if(attemptPlacement(testPiece, lastPiece, openConnector)) return true;
    // }


    // Random version!
    // Look for pieces that can be placed
    for(int i : getRandomIterable(pieces.size())) {
        Piece& testPiece = pieces.at(i);

        if(testPiece.isUsed()) continue; // Skip pieces already placed

        // Checks whether the piece has already been tested
        if(previouslyTested.count(testPiece.getId())) continue;
        previouslyTested.insert(testPiece.getId());

        // Attempt placement of this piece.
        if(attemptPlacement(testPiece, lastPiece, openConnector)) return true;

        if(testPiece.isFlippable()) {
            testPiece.flip();
        }

        // Re-attempt placement of this piece after flipping.
        if(attemptPlacement(testPiece, lastPiece, openConnector)) return true;
    }



    // No track was found for any placeable piece.
    return false;
}

bool Track::attemptPlacement(Piece& testPiece, const Piece& lastPiece, Connector& openConnector) {
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
        for(const Piece& testCollisionPiece : pieces) {
            if(!testCollisionPiece.isUsed()) continue; // Skip unplaced pieces.
            // Skip collision checking between consecutive pieces.
            if(&testCollisionPiece == &lastPiece) continue;
            if (testPiece.collides(testCollisionPiece)) {
                noCollision = false;
                break; // Test piece can not be placed.
            }
        }

        if(noCollision) {
            // Place this piece!
            testPiece.setUsed(true);
            nbPiecesPlaced += 1;

            // Link the two connectors together
            testCon.link(openConnector);

            // Checks whether this piece has another free connector
            if(!testPiece.hasOpenConnectors()) return true; // This piece has no more available connectors. The track is built.
            Connector openCon = testPiece.getOpenConnector(); // Get the open connector

            // Checks whether the validation conditions are met between the validation connector and the test piece's open connector.
            if(openCon.validate(*validationConnector, validationAngle, validationDist)) {

                if(nbPiecesPlaced >= minPieceNb) return true; // Track is closed!
            }
            
            // Place the next piece.
            if( generateTrack(testPiece, openCon) ) {
                // The track was built! return true.
                return true;
            }

            else {
                // The track could not be build. Unlink and remove piece.
                testPiece.setUsed(false);
                nbPiecesPlaced -= 1;
                testCon.unlink(openConnector);
            }
        }
    }
    return false; // Piece placement was unsuccessful.
}


void Track::writeToFile() const {
    // Write result to the result file
    std::ofstream file("../tracks/track_result.json");

    file << "{\"pieces\":[";

    for(uint i = 0; i < pieces.size(); i++) {
        file << pieces[i].toJson();
        if(i < pieces.size() - 1) file << ",";
    }

    file << "]}";

    file.close();
}

std::vector<int> Track::getRandomIterable(uint l) {
    std::vector<int> res;
    for(uint i = 0; i < l; i ++) {
        res.push_back(i);
    }
    // Shuffle the vector using the random engine
    std::shuffle(std::begin(res), std::end(res), randomEngine);
    return res;
}