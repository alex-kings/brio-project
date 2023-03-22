#pragma once
#include <json/json.h>
#include <vector>
#include "Piece.h"
#include <random>
#include <chrono>
#include <stack>

/** 
 * Class for the generation of tracks.
*/

class Circuit {

private:

    /**
     * Global members
    */
    std::vector<Piece> pieces;
    std::default_random_engine randomEngine;
    // The maximum number of loops this circuit can theoretically have.
    int maxLoops;
    // The start time of the entire generation.
    std::chrono::steady_clock::time_point startTime;
    unsigned int nbPiecesPlaced = 0;

    // Validation conditions
    float validationAngle;
    float validationDist;
    // float halfMaxDist; // Half of the maximum linear distance of this track.
    bool isTwoLevel;


    /**
     * Current Loop members
    */
    int currentLoop;
    int availableEnd; // The index of the end of the section of available pieces.
    std::stack<int> pEnds; // The previously used placedEnd's.

    // The previously used validation conditions
    struct validationCondition {
        Piece* startPiece;
        Connector* startConnector;
        Piece* validationPiece;
        Connector* validationConnector;

        // Construct a validationCondition.
        validationCondition(Piece* sPiece, Connector* sCon, Piece* vPiece, Connector* vCon) {
            startPiece = sPiece;
            startConnector = sCon;
            validationPiece = vPiece;
            validationConnector = vCon;
        }
    };
    std::stack<validationCondition> validationConditions;
    // The distance available in the current set of track pieces.
    float availableDist;


    // Recursion information.
    unsigned int generationCount = 0;
    const unsigned int maxGenerations = 100; // 100
    // The maximum level this track can go to.
    int maxLevelLoop; // The max level this loop can go to.
    unsigned int minPieceNb;

    /**
     * Current Loop iteration members
    */
    const unsigned int maxNumberRecursions = 10000; // 10 000
    unsigned int currentNumberRecursions = 0; 


    // The unused connectors
    // std::vector<std::pair<Piece*, Connector*>> unusedConnectors;

    /**
     * Recursively connects pieces together to generate a closed track.
     * The validation conditions need to be met for the track to be successfully built.
    */
    bool generateLoop(const Piece& lastPiece, Connector& openConnector);

    /**
     * Attempt placement of the given piece.
    */
    bool attemptPlacement(Piece& testPiece, const Piece& lastPiece, Connector& openConnector);

    /**
     * Provides a vector of ints going from start to end-1 in random order.
    */
    std::vector<int> getRandomIterable(int start, int end) ;

    /**
     * Shuffles the available and unreachable pieces around.
    */
    void shufflePieces();

    /**
     * Shuffles the available pieces around.
    */
    void shufflePiecesIteration();

    /**
     * Re-shuffles the available pieces around and mark all pieces as unused. Restarts the timer.
     * Keeps the placed pieces intact.
    */
    void resetIteration();

    /**
     * Sets up the conditions for the coming loop.
    */
    bool setupLoop();

    /**
     * Resets the pieces to the state they were during previous loop genration.
    */
    void resetPreviousLoop();

    /**
     * Sets up the conditions for the generation.
    */
    void setupInitialValidationConditions();

    /**
     * Determines whether there are pieces placed in between c1 and c2
    */
    // bool piecesInBetween(const Connector& c1, const Connector& c2) const;

    /**
     * Launches generations of the current loop.
    */
    bool launchLoopGenerations();

    /**
     * Sanitises the set of track pieces before starting the generation.
     * Sanitisation includes keeping only a pair number of ascending pieces, as well as
     * a pair number of 3-connector pieces.
     * Also determines the number of loops the circuit can have.
    */
    void sanitise();
    
    /**
     * Sets the indexes of placedEnd and availableEnd at the right location
    */
    void setIndexLocations();

    /**
     * Sorts the pieces so that all the placed pieces are located at the front of the piece array
    */
    void putUsedPiecesInFront();

    /**
     * Sets the startPiece and startConnector, as well as the validationPiece and validationConnector.
    */
    bool setValidationConditions();

    /**
     * Sanitise the current loop: must contain the correct number of 3con pieces, as well as a pair number of going up/down pieces.
    */
    void sanitiseLoop();
    
    /**
     * Print the current track state to console. Used for debug.
    */
    void printTrack();

    /**
     * Create a part between the two given open connectors.
     * Check whether this part collides with any of the placed pieces,
     * ignoring collisions with the two given pieces
    */
    bool noPiecesInBetween(const Piece& p1, const Piece& p2, const Connector c1, const Connector c2) const;


public:
    /**
     * Construct from a selection of pieces.
     * Puts maxLevel to be one if the track is specified to be two level only.
     * vCondition is either loose, medium or close.
    */
    Circuit(std::vector<Piece> allPieces, const int seed, const bool isTwoLevel, const std::string vCondition);

    /**
     * Generate the circuit
    */
    bool generate();

    std::string getTrackJson() const {
        std::string result;
        result += "{\"pieces\":[";
        for(unsigned int i = 0; i < pieces.size(); i++) {
            result += pieces[i].toJson();
            if(i < pieces.size() - 1) result += ",";
        }
        result += "]}";
        return result;
    }
};