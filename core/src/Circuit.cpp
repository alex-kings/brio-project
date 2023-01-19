#include "Circuit.h"
#include "ParseJson.h"
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <math.h>

Circuit::Circuit(std::vector<Piece> allPieces, const int seed, const bool isTwoLevel) {
    // Generate random engine with seed. If seed is -1, the current time is used for the seed.
    if(seed == -1) this->randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    else this->randomEngine = std::default_random_engine(seed); 
    this->pieces = allPieces;

    sanitise();

    // Initial generation conditions.
    placedEnd = 1; // The first piece is already placed.
    availableEnd = pieces.size();
    this->startPiece = &pieces.at(0);
    this->validationPiece = &pieces.at(0);
    std::vector<Connector*> cons = startPiece->getOpenConnectors();
    this->startConnector = cons[0];
    this->validationConnector = cons[1];
    startPiece->setUsed(true);


    // Validation conditions
    this->validationAngle = 0.4*M_PI;
    // validationDist = pieces.size() * 5;
    this->validationDist = 200;
    // minPieceNb = std::floor(pieces.size()*0.6); // 60% of pieces
    this->minPieceNb = 0.6*(availableEnd - placedEnd); // 60% of the pieces available for generation in the current loop.

    if(isTwoLevel) {
        this->maxLevel = 1;
    }
    else {
        this->calculateMaxLevel(); // Find the track's maximum level
    }

    // Start with a random arrangement of the original set of pieces.
    this->shufflePieces();
}

bool Circuit::generate() {
    // Start timer
    this->startTime = std::chrono::steady_clock::now();

    std::cout << "Maximum number of loops: "<< remainingLoops << "\n";

    // return launchLoopGenerations();
    
    int loopCount = 0;

    while(remainingLoops > 0) {
        loopCount++;
        std::cout << "Starting generation for loop " << loopCount << "\n";

        if(!launchLoopGenerations()) {
            // The generation did not work.
            return false;
        }

        std::cout << "First loop generated successfully! Going for second.\n";

        // Generation of the loop successful.
        remainingLoops --;

        if(remainingLoops > 0) {
            std::cout << "Preparing for second loop\n";
            // Prepare next generation
            putUsedPiecesInFront();
            std::cout << "Pieces placed have been put in front.\n";
            setIndexLocations(remainingLoops);
            std::cout << "Index locations have been set up.\n";
            std::cout << "placedEnd: " << placedEnd << " availableEnd: " << availableEnd << "\n";
            // Change the start and validation pieces and connectors
            setValidationConditions();
            std::cout << "Validation conditions set up.\n";
            // Reset generation count.
            this->generationCount = 0;
        }
    }

    // return launchLoopGenerations();
    return true;
}

bool Circuit::launchLoopGenerations() {
    while(true) {
        this->generationCount++;
        std::cout<<"Generation " << this->generationCount << " starting.\n";

        // Generate the track!
        if(this->generateLoop((*startPiece), (*startConnector))) {
            return true;
        }

        std::cout << "Generation " << this->generationCount << " unsuccessful after " << this->currentNumberRecursions << " recursions.\n";

        // Check if the maximum number of generations has been reached.
        if(this->generationCount >= this->maxGenerations) return false;

        // Track could not be generated.
        this->reset();
    }
}

bool Circuit::generateLoop(const Piece& lastPiece, Connector& openConnector) {
    this->currentNumberRecursions ++;
    if(this->currentNumberRecursions >= this->maxNumberRecursions) return false;

    // Keep track of previously tested pieces
    std::unordered_set<std::string> previouslyTested;

    // Look for pieces that can be placed
    for(int i : this->getRandomIterable(this->placedEnd, this->availableEnd)) {
        Piece& testPiece = this->pieces.at(i);

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
    return false;
}

bool Circuit::attemptPlacement(Piece& testPiece, const Piece& lastPiece, Connector& openConnector) {
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

        // If the piece is a level piece, test that it does not go below zero or above the max level before placement.
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

        // Checks whether the test piece collides with any placed piece.
        bool noCollision = true;
        for(int i = 0; i < availableEnd; i++) {
            const Piece& testCollisionPiece = this->pieces.at(i);

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
            if(freeConnectors.size() == 0) {
                // This piece has no more available connectors. The track is built.
                std::cout << "No more free connectors!\n";
                return true;
            };

            Connector& openCon = *(freeConnectors.at(0)); // Get the open connector

            // Checks whether the validation conditions are met between the validation connector and the test piece's open connector.
            if(nbPiecesPlaced >= minPieceNb && openCon.validate(*validationConnector, validationAngle, validationDist)) {
                // Tests if there are pieces in between the two validation connectors
                // if(!piecesInBetween(openCon, *validationConnector)) {
                // }

                // Check that a pair number of 3 connector pieces is placed.
                // int numberThreeConPlaced = 0;
                // for(Piece& piece : this->pieces) {
                //     if(piece.isUsed() && (piece.getId() == "L" || piece.getId() == "M")) {    
                //         numberThreeConPlaced ++;
                //     }
                // }
                // if(numberThreeConPlaced % 2 == 0 && this->allThreeConPlaced()) {
                //     // Track is closed!
                //     openCon.link(*validationConnector);
                //     return true; 
                // }
                // Track is closed!
                openCon.link(*validationConnector);
                return true; 
            }

            // Place the next piece.
            if( generateLoop(testPiece, openCon) ) {
                // The track got built! return true.
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

std::vector<int> Circuit::getRandomIterable(int start, int end) {
    std::vector<int> res;
    for(int i = start; i < end; i ++) {
        res.push_back(i);
    }
    // Shuffle the vector using the random engine
    std::shuffle(res.begin(), res.end(), this->randomEngine);
    return res;
}

void Circuit::shufflePieces() {
    try{
        std::shuffle(pieces.begin() + placedEnd, pieces.begin() + availableEnd, this->randomEngine);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    // Ensure there are exactly two threecon pieces
    // ensureCorrectNumberThreeCon();
}

void Circuit::reset() {
    // Shuffle pieces around
    shufflePieces();
    // Reset number pieces placed
    nbPiecesPlaced = 0;
    // Reset number of recursions in this generation
    currentNumberRecursions = 0;
}

void Circuit::setupLoop() {
    // Position the placedEnd and availableEnd indices.
    setIndexLocations(remainingLoops);

    // Shuffle all the unplaced pieces around
    shufflePieces();
}

void Circuit::calculateMaxLevel() {
    int numberAscending = 0;
    for(const Piece& p : this->pieces) {
        if(p.getId() == "N") numberAscending ++;
    }
    this->maxLevel = this->startConnector->getLevel() + (numberAscending / 2);
}

void Circuit::sanitise() {
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
                break;
            }
        }
    }
    if((mPieces + lPieces) % 2 != 0) {
        const std::string removeId = (mPieces > lPieces ? "M" : "L");
        for(auto it = this->pieces.begin(); it != this->pieces.end(); ++it) {
            if(it->getId() == removeId) {
                // Remove it from the vector.
                this->pieces.erase(it);
                break;
            }
        }
    }

    // Determine the number of loops this circuit can have
    int threeConPieces = 0;
    for(const Piece& p : pieces) {
        if(p.getId() == "M" || p.getId() == "L") threeConPieces ++;
    }
    this->remainingLoops = threeConPieces / 2 + 1;
}

void Circuit::setIndexLocations(int remainingLoops) {
    // Finds the position of the last piece placed.
    for(unsigned int i = 0; i < pieces.size(); i++) {
        if(!pieces[i].isUsed()) {
            // Piece is not used.
            placedEnd = i;
            break;
        }
    }
    // Divides the total number of available pieces with the number of remaining loops to find the number of available pieces for the next loop
    availableEnd = (pieces.size() - placedEnd) / remainingLoops;

    if(placedEnd > availableEnd) std::cerr<<"Placed pieces are higher than available pieces.\n";
    if(availableEnd > (int)pieces.size()) std::cerr << "Available pieces are higher than size of vector.\n";
}

void Circuit::putUsedPiecesInFront() {
    // Sorting condition depends on the use of the pieces.
    std::sort(pieces.begin(), pieces.end(), [](Piece& a, Piece& b) {
        if(!a.isUsed() && b.isUsed()) return false;
        return true;
    });
}

void Circuit::setValidationConditions() {
    std::vector<Piece*> openConPiece;
    // Finds the two open connectors in the placed pieces.
    for(int i = 0; i < placedEnd; i++) {
        if(pieces[i].hasOpenConnector()) {
            openConPiece.push_back(&(pieces[i]));
        }
    }

    // Check that openConPiece has exactly two pieces
    if(openConPiece.size() != 2) std::cerr << "There aren't exactly two open connectors available!\n";
    else std::cout << "Validation conditions could be set for the second loop! :)\n";
    
    // ASSUMES THAT THERE ARE ALWAYS ONLY TWO OPEN CONNECTOR PIECES!
    this->startPiece = openConPiece[0];
    this->startConnector = &(startPiece->getOpenConnector());
    this->validationPiece = openConPiece[1];
    this->validationConnector = &(validationPiece->getOpenConnector());

    // Set the min number of pieces
    minPieceNb = 0.6 * (availableEnd - placedEnd);
}

void Circuit::ensureCorrectNumberThreeCon() {
    std::cout << "Ensuring the correct number of 3 connector pieces in the list of available pieces.\n";
    if(remainingLoops == 1) return;
    
    // Ensure that there are EXACTLY two 3-con pieces in the set of available pieces for the coming generation.
    int numberThreeCon = 0;
    for(int i = placedEnd; i < availableEnd; i++) {
        if(pieces[i].getId() == "M" || pieces[i].getId() == "L") {
            numberThreeCon ++;
        }
    }
    while(numberThreeCon != 2) {
        // The indices of the pieces to swap.
        int index1=-1;
        int index2=-1;

        if(numberThreeCon < 2) {
            // Need to get an extra 3con piece.
            numberThreeCon++;

            // Find 3con piece in the list of unavailable pieces.
            for(unsigned int i = availableEnd; i < pieces.size(); i++) {
                if(pieces[i].getId() == "M" || pieces[i].getId() == "L") {
                    index1 = i;
                    break;
                }
            }
            // Find the index of a non-3con piece in the list of available pieces
            for(int j = placedEnd; j < availableEnd; j++) {
                if(pieces[j].getId() != "M" && pieces[j].getId() != "L") {
                    index2 = j;
                }
                break;
            }
        }
        else{
            // Need to get rid of a 3con piece.
            numberThreeCon--;

            // Find a 3con piece in the list of available pieces
            for(int i = placedEnd; i < availableEnd; i++) {
                if(pieces[i].getId() == "M" || pieces[i].getId() == "L") {
                    index1 = i;
                    break;
                }
            }
            // Find the index of a non-3con piece in the list of unavailable pieces
            for(unsigned int j = availableEnd; j < pieces.size(); j++) {
                if(pieces[j].getId() != "M" && pieces[j].getId() != "L") {
                    index2 = j;
                }
                break;
            }
        }
        // Swap the pieces at the indices we got
        std::iter_swap(pieces.begin() + index1, pieces.begin() + index2);
    }

    // Check that it is correct
    int n3c = 0;
    for(int i = placedEnd; i < availableEnd; i++) {
        if(pieces[i].getId() == "M" || pieces[i].getId() == "L") {
            n3c ++;
        }
    }
    if(n3c != 2) std::cerr << "Ensuring there are exactly 2 threecon pieces in available set did not work.\n";
}


// // bool Circuit::piecesInBetween(const Connector& c1, const Connector& c2) const {
// //     for(const Piece& p : this->pieces) {
// //         if(!p.isUsed()) continue;
// //         if(p.intersects(c1.getPosition(), c2.getPosition())) return true;
// //     }
// //     return false;
// // }
