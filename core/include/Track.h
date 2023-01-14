#pragma once
#include <json/json.h>
#include <vector>
#include "Piece.h"
#include <random>
#include <chrono>

/** 
 * Class for the generation of tracks.
*/

class Track {

private:
    std::vector<Piece> pieces;

    // Random engine
    std::default_random_engine randomEngine;

    // UNCOMMENT FOR A TIME-SEEDED RANDOM ENGINE
    // std::default_random_engine randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    // The start date of the entire generation
    std::chrono::steady_clock::time_point startTime;

    // The number of times the generation starts from scratch.
    unsigned int generationCount = 0;

    // Maximum number of generations before stopping
    const unsigned int maxGenerations = 100;

    // Max number of iterations before starting the next generation
    const unsigned int maxNumberRecursions = 10000; // 10 000

    // Number of recursive steps in this generation
    unsigned int currentNumberRecursions = 0; 

    // Number of pieces placed
    unsigned int nbPiecesPlaced = 0;

    // The maximum level this track can go to.
    int maxLevel;

    // The initial piece
    Piece* firstPiece;
    Connector* validationConnector;

    std::vector<Connector*> validationConnectors;


    // Validation conditions
    float validationAngle;
    float validationDist;
    unsigned int minPieceNb;
    float halfMaxDist; // Half of the maximum linear distance of this track.

    /**
     * Recursively connects pieces together to generate a closed track.
     * The validation conditions need to be met for the track to be successfully built.
    */
    bool generateTrack(const Piece& lastPiece, Connector& openConnector);

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
    */
    void reset();

    /**
     * Returns the maximum linear distance of this track.
    */
    float getMaxDist() const;

    /**
     * Determines whether there are pieces placed in between c1 and c2
    */
    bool piecesInBetween(const Connector& c1, const Connector& c2) const;

    /**
     * Sanitises the set of track pieces before starting the generation.
     * Sanitisation includes keeping only a pair number of ascending pieces, as well as
     * a pair number of 3-connector pieces.
    */
    void sanitise();

    /**
     * Calculates the maximum level the track can go to by checking the number of ascending pieces in the set.
    */
    void calculateMaxLevel();

public:
    /**
     * Construct from a selection of pieces.
     * Generate track with a random seed if the specified seed is -1.
     * Puts maxLevel to be one if the track is specified to be two level only.
    */
    Track(const std::vector<Piece> availablePieces, const int seed, const bool isTwoLevel);


    /**
     * Write result track to output file (OUTDATED)
    */
    void writeToFile() const;

    /**
     * Generate the track
    */
    bool generate();

    /**
     * Get the Json representation of this track.
    */
    std::string getTrackJson() const;
};
