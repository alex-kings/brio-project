#pragma once
#include <json/json.h>
#include <vector>
#include "Piece.h"
#include <random>
#include <chrono>

/** 
 * Class for the generation of tracks.
*/

class Loop {

private:
    std::vector<Piece> pieces;
    int placedEnd; // The index of the end of the section of placed pieces.
    int availableEnd; // The index of the end of the section of available pieces.
    std::default_random_engine randomEngine;

    // The start piece
    Piece* startPiece;
    const Connector* startConnector;

    // The validation piece
    Piece* validationPiece;
    const Connector* validationConnector;

    // The start date of the entire generation
    std::chrono::steady_clock::time_point startTime;

    // Recursion information.
    unsigned int generationCount = 0;
    const unsigned int maxGenerations = 100; // 100
    const unsigned int maxNumberRecursions = 10000; // 10 000
    unsigned int currentNumberRecursions = 0; 

    unsigned int nbPiecesPlaced = 0;

    // The maximum level this track can go to.
    int maxLevel;

    // The unused connectors
    std::vector<std::pair<Piece*, Connector*>> unusedConnectors;

    // Validation conditions
    float validationAngle;
    float validationDist;
    unsigned int minPieceNb;
    float halfMaxDist; // Half of the maximum linear distance of this track.

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
     * Provides a random vector of length l containing all integers from 0 to l-1.
    */
    std::vector<int> getRandomIterable(unsigned int l) ;

    /**
     * Shuffles pieces in random order.
    */
    void shufflePieces();

    /**
     * Re-shuffles the available pieces around and mark all pieces as unused. Restarts the timer.
     * Keeps the placed pieces intact.
    */
    void reset();

    /**
     * Determines whether there are pieces placed in between c1 and c2
    */
    // bool piecesInBetween(const Connector& c1, const Connector& c2) const;

    /**
     * Calculates the maximum level the track can go to by checking the number of ascending pieces in the set.
    */
    void calculateMaxLevel();

    /**
     * Tells whether all the 3 connector pieces are placed.
    */
    bool allThreeConPlaced() const;
    
public:
    /**
     * Construct from a selection of pieces.
     * Puts maxLevel to be one if the track is specified to be two level only.
    */
    Loop(const std::vector<Piece> allPieces, const int placedEnd, const int availableEnd, Piece* sPiece, Piece* vPiece, const std::default_random_engine engine, const bool isTwoLevel);

    /**
     * Generate the Loop
    */
    bool generate();
};