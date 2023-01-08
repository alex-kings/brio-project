#include "Track.h"
#include "ParseJson.h"
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <math.h>

Track::Track(const std::vector<Piece> availablePieces) {
    pieces = availablePieces;

    // Validation conditions
    validationAngle = 0.2*M_PI;
    validationDist = pieces.size() * 5;
    minPieceNb = std::floor(pieces.size()*0.6); // 60% of pieces
    halfMaxDist = getMaxDist() / 2;

    shufflePieces();

    // Start timer
    startTime = std::chrono::steady_clock::now();

    uint generationCount = 0;

    while(true) {
        generationCount++;

        // Get first piece from available pieces.
        firstPiece = &pieces.back();
        validationConnector = &firstPiece->getConnector(1);

        // Place the initial piece at the origin
        firstPiece->placeAtOrigin();

        // Mark this piece as used.
        firstPiece->setUsed(true);

        // Generate the track!
        if(generateTrack((*firstPiece), firstPiece->getConnector(0))) break; // Track generated!

        // Track could not be generated.
        this->reset();
    }

    std::cout << "Generated after " << generationCount << " generations\n";

    // Write result to file
    writeToFile();

    std::cout << "Recursions: " << std::to_string(count) << "\n";

}


bool Track::generateTrack(const Piece& lastPiece, Connector& openConnector) {
    count ++;

    // Check if timer ran out
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

    if(elapsed > maxTime) {
        // Took too long.
        return false; 
    }

    // Keep track of previously tested pieces
    std::unordered_set<std::string> previouslyTested;

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

        // Check "half-max-linear-distance" heuristic
        if(firstPiece->getConnector(0).getPosition().euclidianDist(testCon.getPosition()) > halfMaxDist) {
            // Piece is further away than the half maximum linear distance
            // std::cout << "Attempted to place a piece too far.\n";
            continue;
        }

        // Check "not too close to start connector" heuristic
        // if(pieces.size() - nbPiecesPlaced != 1) {
        //     if(validationConnector->getPosition().euclidianDist())
        // }

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
                // Tests if there are pieces in between the two validation connectors
                // if(!piecesInBetween(openCon, *validationConnector)) {
                // }
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

void Track::shufflePieces() {
    std::shuffle(pieces.begin(), pieces.end(), randomEngine);
}

void Track::reset() {
    // Shuffle pieces around
    shufflePieces();
    // Mark all pieces as unused
    for(Piece& piece : pieces) {
        piece.setUsed(false);
    }
    // Reset number pieces placed
    nbPiecesPlaced = 0;
    // Reset timer
    startTime = std::chrono::steady_clock::now();
}

float Track::getMaxDist() const {
    float tot = 0;
    for(const Piece& piece: pieces) {
        tot += piece.getDist();
    }
    return tot;
}

bool Track::piecesInBetween(const Connector& c1, const Connector& c2) const {
    for(const Piece& p : this->pieces) {
        if(!p.isUsed()) continue;
        if(p.intersects(c1.getPosition(), c2.getPosition())) return true;
    }
    return false;
}