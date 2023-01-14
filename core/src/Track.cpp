#include "Track.h"
#include "ParseJson.h"
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <math.h>

Track::Track(const std::vector<Piece> availablePieces, const int seed, const bool isTwoLevel) {
    pieces = availablePieces;

    // Generate random engine with seed. If seed is -1, the current time is used for the seed.
    if(seed == -1) this->randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    else this->randomEngine = std::default_random_engine(seed);

    // Validation conditions
    validationAngle = 0.2*M_PI;
    // validationDist = pieces.size() * 5;
    validationDist = 300;
    // minPieceNb = std::floor(pieces.size()*0.6); // 60% of pieces
    minPieceNb = 8;
    halfMaxDist = getMaxDist() / 2;

    this->sanitise(); // Sanitise pieces : remove the odd number of ascending and/or 3-connector piece.
    if(isTwoLevel) {
        this->maxLevel = 1;
    }
    else {
        this->calculateMaxLevel(); // Find the track's maximum level
    }

    std::cout << "Max level is " << this->maxLevel << "\n";

    // Start with a random arrangement of the original set of pieces.
    shufflePieces();
}

bool Track::generate() {
    // Start timer for the entier generation
    this->startTime = std::chrono::steady_clock::now();

    while(true) {
        this->generationCount++;
        std::cout<<"Generation " << generationCount << " starting.\n";

        // Get first piece from available pieces.
        firstPiece = &pieces.back();
        validationConnector = &firstPiece->getConnector(1);

        // Add the original validation connector.
        validationConnectors.push_back(&firstPiece->getConnector(1));

        // Make sure the first piece is placed at level zero
        if(firstPiece->getLowestLevel() != 0) {
            firstPiece->changeLevel(-(firstPiece->getLowestLevel()));
        }

        // Place the initial piece at the origin
        firstPiece->placeAtOrigin();

        // Mark this piece as used.
        firstPiece->setUsed(true);

        // Generate the track!
        if(generateTrack((*firstPiece), firstPiece->getConnector(0))) {
            std::cout << "Generated after " << generationCount << " generations\n";
            return true;
        }

        std::cout << "Generation " << generationCount << " unsuccessful after " << currentNumberRecursions << " recursions.\n";

        // Check if the maximum number of generations has been reached.
        if(generationCount >= maxGenerations) return false;

        // Track could not be generated.
        this->reset();
    }
}

bool Track::generateTrack(const Piece& lastPiece, Connector& openConnector) {
    currentNumberRecursions ++;

    if(currentNumberRecursions >= maxNumberRecursions) return false; // Unsuccessful generation.

    // Keep track of previously tested pieces
    std::unordered_set<std::string> previouslyTested;

    // Look for pieces that can be placed
    for(int i : getRandomIterable(pieces.size())) {
        Piece& testPiece = pieces.at(i);

        if(testPiece.isUsed()) continue; // Skip pieces already placed

        // Checks whether the piece has already been tested
        if(previouslyTested.count(testPiece.getId())) continue;
        previouslyTested.insert(testPiece.getId());

        // Randomly pick whether we should start with the piece going left or right
        if (randomEngine() % 2 == 1) testPiece.flip(); 

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
    for(unsigned int j = 0; j < testPiece.getNumberConnectors(); j++) {
        Connector& testCon = testPiece.getConnector(j);

        if(!testCon.isFree()) continue; // The connector is not free.
        if(testCon.getType() == openConnector.getType()) continue; // Skip connectors of the same type.

        // Checks whether the connectors are at the same height.
        if(testCon.getLevel() != openConnector.getLevel()) {
            // Try to readjust the piece at the level of the open connector
            int levelDiff = openConnector.getLevel() - testCon.getLevel();
            testPiece.changeLevel(levelDiff);
        }

        // If the piece is a level piece, test that it does not go below zero before placement.
        if(testPiece.getId() == "N") {
            int pieceLevel = testPiece.getLowestLevel();
            // Check that the piece is not below 0 or above the max level
            if(pieceLevel < 0  || pieceLevel >= maxLevel) continue;
        }

        // Angle and position difference between the two connectors.            
        float angleDiff = openConnector.getDirection().getAngleDifference(testCon.getDirection());
        Vec2D positionDiff = openConnector.getPosition() - testCon.getPosition();

        // Connects the pieces around their two connectors.
        testPiece.rotate(testCon.getPosition(), M_PI - angleDiff);
        testPiece.translate(positionDiff);

        // // Check "half-max-linear-distance" heuristic
        // if(firstPiece->getConnector(0).getPosition().euclidianDist(testCon.getPosition()) > halfMaxDist) {
        //     // Piece is further away than the half maximum linear distance
        //     // std::cout << "Attempted to place a piece too far.\n";
        //     continue;
        // }

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
            std::vector<Connector*> freeConnectors = testPiece.getOpenConnectors();
            if(freeConnectors.size() == 0) return true; // This piece has no more available connectors. The track is built.

            // One single open connector.
            if(freeConnectors.size() == 1) {
                Connector& openCon = *(freeConnectors.at(0)); // Get the open connector

                // Checks whether the validation conditions are met between the validation connector and the test piece's open connector.
                // if(openCon.validate(*validationConnector, validationAngle, validationDist)) {
                //     // Tests if there are pieces in between the two validation connectors
                //     // if(!piecesInBetween(openCon, *validationConnector)) {
                //     // }
                //     if(nbPiecesPlaced >= minPieceNb) return true; // Track is closed!
                // }

                // Check validation conditions against all the validation connectors.
                if(nbPiecesPlaced >= minPieceNb) {
                    for(Connector* validationCon : validationConnectors) {
                        if(openCon.validate(*validationCon, validationAngle, validationDist)) return true;
                    }
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
            // 3 connector piece.
            else if(freeConnectors.size() == 2) {
                // NEED TO ADD CASE WHEN THE MULTI-CONNECTOR PIECE VALIDATES ALL OF ITS FREE CONNECTORS ON PLACEMENT
                // AND VALIDATION IN ANY CASE FOR A MULTI CONNECTOR PIECE.

                Connector& freeCon1 = *(freeConnectors.at(0));
                Connector& freeCon2 = *(freeConnectors.at(1));

                // Attempt placement of pieces for the two free connectors.
                if(generateTrack(testPiece, freeCon1) && generateTrack(testPiece, freeCon2)) {
                    return true;
                }
                else {
                    nbPiecesPlaced -=1;
                    testPiece.setUsed(false);
                    testCon.unlink(openConnector);
                }
            }
        }
    }
    return false; // Piece placement was unsuccessful.
}


void Track::writeToFile() const {
    // Write result to the result file
    std::ofstream file("../tracks/track_result.json");

    file << "{\"pieces\":[";

    for(unsigned int i = 0; i < pieces.size(); i++) {
        file << pieces[i].toJson();
        if(i < pieces.size() - 1) file << ",";
    }

    file << "]}";

    file.close();
}

std::vector<int> Track::getRandomIterable(unsigned int l) {
    std::vector<int> res;
    for(unsigned int i = 0; i < l; i ++) {
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
    // Reset number of recursions in this generation
    currentNumberRecursions = 0;
    // Reset validation connectors
    validationConnectors.clear();
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

std::string Track::getTrackJson() const {
    
    std::string result;
    result += "{\"pieces\":[";

    for(unsigned int i = 0; i < pieces.size(); i++) {
        result += pieces[i].toJson();
        if(i < pieces.size() - 1) result += ",";
    }

    result += "]}";

    return result;
}

void Track::sanitise() {
    int ascendingPieces = 0;
    int mPieces = 0;
    int lPieces = 0;
    for(const Piece& piece : this->pieces) {
        const std::string pieceId = piece.getId();
        if(pieceId == "N") ascendingPieces++;
        if(pieceId == "M") mPieces++;
        if(pieceId == "L") lPieces++;
    }
    if(ascendingPieces % 2 != 0) {
        // Odd number of ascending pieces: getting rid of one.
        for(auto it = this->pieces.begin(); it != this->pieces.end(); ++it) {
            if(it->getId() == "N") {
                // Remove it from the vector.
                this->pieces.erase(it);
                return;
            }
        }
    }
    if((mPieces + lPieces) % 2 != 0) {
        const std::string removeId = (mPieces > lPieces ? "M" : "L");
        for(auto it = this->pieces.begin(); it != this->pieces.end(); ++it) {
            if(it->getId() == removeId) {
                // Remove it from the vector.
                this->pieces.erase(it);
                return;
            }
        }
    }
}

void Track::calculateMaxLevel() {
    int numberAscending = 0;
    for(const Piece& p : this->pieces) {
        if(p.getId() == "N") numberAscending ++;
    }
    this->maxLevel = numberAscending / 2;
}
