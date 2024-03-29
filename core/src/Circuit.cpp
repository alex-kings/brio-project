#include "Circuit.h"
#include "ParseJson.h"
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <math.h>

Circuit::Circuit(std::vector<Piece> allPieces, const int seed, const bool isTwoLevel, const std::string vCondition) {
    // Generate random engine with seed. If seed is -1, the current time is used for the seed.
    if(seed == -1) this->randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    else this->randomEngine = std::default_random_engine(seed); 
    this->pieces = allPieces;

    sanitise();

    // Validation conditions
    if(vCondition == "loose") {
        // Loose validation conditions.
        validationAngle = 0.4*M_PI;
        validationDist = 300;
    }
    else if(vCondition == "medium") {
        // Loose validation conditions.
        validationAngle = 0.3*M_PI;
        validationDist = 200;
    }
    else if(vCondition == "close") {
        // Loose validation conditions.
        validationAngle = 0.2*M_PI;
        validationDist = 100;
    }
    else {
        // Validation condition is something else
        validationAngle = 0.3*M_PI;
        validationDist = 300;
    }
    
    this->isTwoLevel = isTwoLevel;

    // Calculate the maximum number of loops
    int numberThreeCon = 0;
    for(Piece& p : pieces) {
        if(p.getId() == "M" || p.getId() == "L") {
            numberThreeCon ++;
        }
    }
    this->maxLoops = numberThreeCon / 2 + 1;

    setupInitialValidationConditions();
}

bool Circuit::generate() {
    // Start timer
    this->startTime = std::chrono::steady_clock::now();

    std::cout << "Maximum number of loops: "<< maxLoops << "\n";
    
    for (currentLoop = 0; currentLoop < maxLoops; currentLoop++) {
        // std::cout << "Starting!\n";
        if(!setupLoop()) {
            // Setup was unsuccessful
            std::cerr << "Setup was not successful!\n";
            return true; // Should return false here.
        }
        if(!launchLoopGenerations()) {
            // Generation of the current loop unsuccessful. Resetting to the previous loop generation.
            if(currentLoop == 0) {
                // Loop 0 failed.
                std::cout << "Failed to build loop 0.\n";
                return false;
            }
            else {
                // Resetting to the previous loop.
                currentLoop --;
                resetPreviousLoop();
                currentLoop --;
            }
        }
    }
    // Print elapsed time.
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::cout   << "Generated in " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() 
                << "ms, " << generationCount << " generations, " << currentNumberRecursions << " recursions\n";

    return true;
}

bool Circuit::launchLoopGenerations() {
    while(true) {
        this->generationCount++;
        // std::cout << "iteration " <<this->generationCount << "; available dist: " << availableDist << "\n";

        // Generate the track!
        if(this->generateLoop(*(validationConditions.top().startPiece), *(validationConditions.top().startConnector))) {
            // std::cout << "Generation " << generationCount << " successful\n";
            return true;
        }

        // std::cout << "Generation " << generationCount << " unsuccessful (loop " << currentLoop << ", it " << currentNumberRecursions << ")\n";

        // Check if the maximum number of generations has been reached.
        if(this->generationCount >= this->maxGenerations) {
            std::cout << "Maximum number of generations has been reached. Couldn't generate the track.\n";
            return false;
            // return true;
        }

        // Track could not be generated.
        this->resetIteration();
    }
}

bool Circuit::generateLoop(const Piece& lastPiece, Connector& openConnector) {
    // std::cout << "Starting iteration " << currentNumberRecursions << " for generation of loop " << currentLoop <<"\n";
    this->currentNumberRecursions ++;
    if(this->currentNumberRecursions >= this->maxNumberRecursions) return false;

    // Keep track of previously tested pieces
    std::unordered_set<std::string> previouslyTested;

    // Look for pieces that can be placed
    // for(int i : this->getRandomIterable(pEnds.top(), this->availableEnd)) {
    for(int i = pEnds.top(); i < availableEnd; i++) {
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
            // Re-attempt placement of this piece after flipping.
            if(attemptPlacement(testPiece, lastPiece, openConnector)) return true;
        }
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
            if(pieceLevel < 0  || pieceLevel >= maxLevelLoop) continue;
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
            availableDist -= testPiece.getDist(); // Remove this piece's distance to the available distance.

            // Link the two connectors together
            testCon.link(openConnector);

            // Checks whether this piece has another free connector
            std::vector<Connector*> freeConnectors = testPiece.getOpenConnectors();
            if(freeConnectors.size() == 0) {
                // This piece has no more available connectors. The track is built.
                // std::cout << "No more free connectors!\n";
                return true;
            };

            Connector& openCon = *(freeConnectors.at(0)); // Get the open connector

            // Checks whether the validation conditions are met between the validation connector and the test piece's open connector.
            //
            if(nbPiecesPlaced >= minPieceNb && openCon.validate(*validationConditions.top().validationConnector, validationAngle, validationDist)
            && noPiecesInBetween(*validationConditions.top().validationPiece, testPiece, *validationConditions.top().validationConnector, openCon)) {
                // Tests if there are pieces in between the two validation connectors
                // if(!piecesInBetween(openCon, *validationConnector)) {
                // }

                // Ensure that if this isn't the last loop, there are EXACTLY two 3con pieces placed in the current loop.
                if(currentLoop + 1 != maxLoops) {
                    int numberThreeConPlaced = 0;
                    for(int i = pEnds.top(); i < availableEnd; i++) {
                        if(pieces[i].isUsed() && (pieces[i].getId() == "L" || pieces[i].getId() == "M")) {
                            numberThreeConPlaced++;
                        }
                    }
                    if(numberThreeConPlaced == 2) {
                        // Track is closed!
                        openCon.link(*validationConditions.top().validationConnector);
                        return true; 
                    }
                }                
                else {
                    // Track is closed!
                    openCon.link(*validationConditions.top().validationConnector);
                    return true; 
                }
            }

            // Track is not validated. Check the max dist heuristic
            float distToSuccess = openCon.getPosition().euclidianDist(validationConditions.top().validationConnector -> getPosition()) - validationDist;

            // WITH THE HEURISTIC
            if(distToSuccess < availableDist) {
                // Place the next piece.
                if(generateLoop(testPiece, openCon) ) {
                    // The track got built! return true.
                    return true;
                }
            }

            // WITHOUT THE HEURISTIC
            // if(generateLoop(testPiece, openCon) ) {
            //     // The track got built! return true.
            //     return true;
            // }
            
            // The track could not be build. Unlink and remove piece.
            testPiece.setUsed(false);
            nbPiecesPlaced -= 1;
            availableDist += testPiece.getDist(); // Add this piece's dist to the total dist.
            testCon.unlink(openConnector);
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
    std::shuffle(pieces.begin() + pEnds.top(), pieces.end(), this->randomEngine);
}

void Circuit::shufflePiecesIteration() {
    std::shuffle(pieces.begin() + pEnds.top(), pieces.begin() + availableEnd, this->randomEngine);
}

void Circuit::resetIteration() {
    shufflePiecesIteration();
    nbPiecesPlaced = pEnds.top();
    currentNumberRecursions = 0;
}

void Circuit::setupInitialValidationConditions() {
    // Start piece conditions
    // this->startPiece = &pieces.at(0);
    // this->validationPiece = &pieces.at(0);
    std::vector<Connector*> cons = pieces[0].getOpenConnectors();
    // this->startConnector = cons[0];
    // this->validationConnector = cons[1];

    validationConditions.emplace(&pieces.at(0), cons[0], &pieces.at(0), cons[1]);

    pieces.at(0).setUsed(true);
    pEnds.push(1); // TEST
    nbPiecesPlaced = 1;
}

bool Circuit::setupLoop() {
    // std::cout << "Setting up for loop "<< currentLoop << "\n";

    // Reset the number of generations.
    generationCount = 0;
    currentNumberRecursions = 0;

    // Ensure all the placed pieces are at the start of the vector
    putUsedPiecesInFront();
    // std::cout << "Used pieces put in front.\n";

    // Position the placed end and availableEnd indices.
    setIndexLocations();
    // std::cout << "Indices setup: "<< pEnds.top() << " and "<< availableEnd<<"\n"; 

    // Shuffles the pieces around.
    shufflePieces();
    // std::cout << "Pieces shuffled\n";

    // Ensuring correct number of 3con and ascending pieces in the loop.
    sanitiseLoop();
    // std::cout << "Pieces sanitised\n";

    // Setup validation conditions
    if(currentLoop != 0) {
        if(!setValidationConditions()) return false;
    }

    // Calculate the max level of this loop.
    // printTrack();
    int numberAscending = 0;
    for(int i = pEnds.top(); i < availableEnd; i++) {
        if(pieces[i].getId() == "N") numberAscending++;
    }
    // std::cout << "Now setting up maxLevel\n";
    maxLevelLoop = (numberAscending / 2) + validationConditions.top().startConnector->getLevel();
    // std::cout << "Max level setup: "<<maxLevelLoop <<"\n";
    // If this is a two level track, setup to level 1 max.
    if(isTwoLevel && maxLevelLoop > 1) maxLevelLoop = 1;

    // Setup minimum pieces placed condition
    this->minPieceNb = std::floor(0.6*(availableEnd - pEnds.top())) + nbPiecesPlaced; // 60% of the available pieces for this loop, plus the already placed pieces.

    // Get maximum distance for this loop
    float totalDist = 0;
    for(int i = pEnds.top(); i < availableEnd; i++) {
        totalDist += pieces[i].getDist();        
    }
    availableDist = totalDist;
    // std::cout << "Available distance for this track: " << availableDist << "\n";

    std::cout << "Pieces ready for loop " << currentLoop << "\n";
    // printTrack();
    return true;
}


void Circuit::resetPreviousLoop() {
    // Get the previous placed end
    std::cout << "Going from generation " << currentLoop+1 << " to generation " << currentLoop <<"\n";
    // std::cout << "Previous placed end: " << pEnds.top() << "\n";
    pEnds.pop();
    // std::cout << "POPPED PENDS\n";
    // std::cout << "Now placed end: " << pEnds.top() << "\n";
    // int previousplaced end = pEnds.top();

    // Empty pieces placed during the last loop generation
    for(int i = pEnds.top(); i < pieces.size(); i++) {
        pieces[i].setUsed(false);
    }
    pEnds.pop(); // Need to pop a second time to avoid duplication of pEnds.

    // Close the validation connectors of the previous loop
    // validationConnector->setConnected(false);
    // startConnector->setConnected(false);
    validationConditions.top().validationConnector->setConnected(false);
    validationConditions.top().startConnector->setConnected(false);
    validationConditions.pop();
    validationConditions.top().validationConnector->setConnected(false);
    validationConditions.top().startConnector->setConnected(false);
    validationConditions.pop(); // Pop a second time.

    // If resetting for loop 0, make sure the first piece's connectors are open
    if(currentLoop == 0) {
        // std::cout << "Setting up the initial validation conditions for the initial generation." << "\n";
        setupInitialValidationConditions();
    }
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
}


void Circuit::setIndexLocations() {
    // Finds the position of the last piece placed.
    for(unsigned int i = 0; i < pieces.size(); i++) {
        if(!pieces[i].isUsed()) {
            // Piece is not used.
            pEnds.push(i);
            // std::cout << "Pushed " << i << " onto pEnds\n";
            break;
        }
    }

    // Divides the total number of available pieces with the number of remaining loops to find the number of available pieces for the next loop
    availableEnd = ((pieces.size() - pEnds.top()) / (maxLoops - currentLoop) ) + pEnds.top();

    if(pEnds.top() > availableEnd) std::cerr<<"Placed pieces are higher than available pieces.\n";
    if(availableEnd > (int)pieces.size()) std::cerr << "Available pieces are higher than size of vector.\n";
}

void Circuit::putUsedPiecesInFront() {
    // Sorting condition depends on the use of the pieces.
    std::sort(pieces.begin(), pieces.end(), 
        [](Piece& a, Piece& b) -> bool {
            if(a.isUsed() == b.isUsed()) return false; // Behaviour like "less than" comparison
            return(a.isUsed());
        }
    );
}

bool Circuit::setValidationConditions() {
    std::vector<Piece*> openConPiece;
    // Finds the two open connectors in the placed pieces.
    for(int i = 0; i < pEnds.top(); i++) {
        if(pieces[i].hasOpenConnector()) {
            openConPiece.push_back(&(pieces[i]));
        }
    }

    // Check that openConPiece has exactly two pieces
    if(openConPiece.size() != 2) {
        std::cerr << "There aren't exactly two open connectors available!\n";
        return false;
    }
    
    // ASSUMES THAT THERE ARE ALWAYS ONLY TWO OPEN CONNECTOR PIECES AT THIS POINT.
    // this->startPiece = openConPiece[0];
    // this->startConnector = &(startPiece->getOpenConnector());
    // this->validationPiece = openConPiece[1];
    // this->validationConnector = &(validationPiece->getOpenConnector());

    validationConditions.emplace(openConPiece[0], &(openConPiece[0]->getOpenConnector()), openConPiece[1], &(openConPiece[1]->getOpenConnector()));

    return true;
}

void Circuit::sanitiseLoop() {
    if(currentLoop + 1 == maxLoops) {
        // Last loop does not need 3 con sanitisation.
        // std::cout << "Last loop does not need sanitisation\n";
        return;
    }

    // Ascending pieces sanitisation.
    int ascendingNumber = 0;
    for(int i = pEnds.top(); i < availableEnd; i++) {
        if(pieces[i].getId() == "N") ascendingNumber++;
    }
    if(ascendingNumber % 2 != 0) {
        // Need to change the number of ascending pieces. Remove one from the set.
        int index1 = 0;
        int index2 = 0;
        
        // Find index of the ascending piece to remove
        for(int i = pEnds.top(); i < availableEnd; i++) {
            if (pieces[i].getId() == "N") {
                index1 = i;
                break;
            }
        }
        // Find index of a non ascending piece in the available pieces.
        for(int i = availableEnd; i < (int)pieces.size(); i++) {
            if(pieces[i].getId() != "N") {
                index2 = i;
                break;
            }
        }

        // Swap the two
        std::iter_swap(pieces.begin() + index1, pieces.begin() + index2);
    }

    // Ensure that there are EXACTLY two 3-con pieces in the set of available pieces for the coming generation.
    int numberThreeCon = 0;
    for(int i = pEnds.top(); i < availableEnd; i++) {
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
            for(int j = pEnds.top(); j < availableEnd; j++) {
                if(pieces[j].getId() != "M" && pieces[j].getId() != "L") {
                    index2 = j;
                    break;
                }
            }
        }
        else{
            // Need to get rid of a 3con piece.
            numberThreeCon--;

            // Find a 3con piece in the list of available pieces
            for(int i = pEnds.top(); i < availableEnd; i++) {
                if(pieces[i].getId() == "M" || pieces[i].getId() == "L") {
                    index1 = i;
                    break;
                }
            }
            // Find the index of a non-3con piece in the list of unavailable pieces
            for(unsigned int j = availableEnd; j < pieces.size(); j++) {
                if(pieces[j].getId() != "M" && pieces[j].getId() != "L") {
                    index2 = j;
                    break;
                }
            }
        }
        // Swap the pieces at the indices we got
        std::iter_swap(pieces.begin() + index1, pieces.begin() + index2);
    }

    // Check that it is correct
    int n3c = 0;
    for(int i = pEnds.top(); i < availableEnd; i++) {
        if(pieces[i].getId() == "M" || pieces[i].getId() == "L") {
            n3c ++;
        }
    }
    if(n3c != 2) std::cerr << "Ensuring there are exactly 2 threecon pieces in available set did not work.\n";
}

void Circuit::printTrack() {
    std::cout << "placed end: " << pEnds.top() << "; AvailableEnd: " << availableEnd << "\n";
    std::cout << "N, ID, USED, 3CON, ASCENDING, AVAIL.CONS.\n";
    for(int i = 0; i < pieces.size(); i++) {
        int n = pieces[i].getOpenConnectors().size();
        bool ascending = pieces[i].getId() == "N";
        bool threeCon = pieces[i].getId() == "L" || pieces[i].getId() == "M";
        std::cout << i << ",  " << pieces[i].getId() << ",  " << pieces[i].isUsed() << ",  " << threeCon << ",  " << ascending << ", " << n<<"\n";
    }
}

bool Circuit::noPiecesInBetween(const Piece& p1, const Piece& p2, const Connector c1, const Connector c2) const {
    // Constructs a temporary Part between the two given Obbs
    Part temp(c1.getPosition(), c2.getPosition(), c1.getLevel());
    // Check collisions between this part and every previously placed piece
    for(int i = 0; i < availableEnd; i++) {
        const Piece& testPiece = this->pieces.at(i);
        if(!testPiece.isUsed() || &testPiece == &p1 || &testPiece == &p2) continue; // Skip unplaced pieces.

        if (testPiece.collidesWithPart(temp)) return false;
    }
    return true;
}

