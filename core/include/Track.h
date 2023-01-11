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
    const unsigned int maxGenerations = 30;

    // Max number of iterations before starting the next generation
    const unsigned int maxNumberRecursions = 500000; // 500 000

    // Number of recursive steps in this generation
    unsigned int currentNumberRecursions = 0; 

    // Number of pieces placed
    unsigned int nbPiecesPlaced = 0;

    // The initial piece
    Piece* firstPiece;
    Connector* validationConnector;


    // Validation conditions
    float validationAngle;
    float validationDist;
    unsigned int minPieceNb;
    float halfMaxDist; // Half of the maximum linear distance of this track.
    // float minDistToStartCon = 

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

public:
    /**
     * Construct from a selection of pieces.
    */
    // Track(const Json::Value& selection);
    Track(const std::vector<Piece> availablePieces, const int seed);


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
