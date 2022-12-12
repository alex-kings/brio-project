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
    std::default_random_engine randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    // The start date of search
    std::chrono::steady_clock::time_point startTime;

    // Max time before starting generation again
    const double maxTime = 1000; // 1 second.

    // Number of recursive steps
    uint count = 0; 
    uint nbPiecesPlaced = 0;

    // The initial piece
    Piece* firstPiece;
    Connector* validationConnector;


    // Validation conditions
    float validationAngle;
    float validationDist;
    uint minPieceNb;
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
    std::vector<int> getRandomIterable(uint l) ;

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

public:
    /**
     * Construct from a selection of pieces.
    */
    // Track(const Json::Value& selection);
    Track(const std::vector<Piece> availablePieces);


    void writeToFile() const;
};