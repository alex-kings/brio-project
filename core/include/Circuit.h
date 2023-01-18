#pragma once
#include <vector>

/**
 * Class representing a circuit of pieces with one or multiple loops.
*/

class Circuit {

private:
    std::vector<Piece> pieces;
    std::default_random_engine randomEngine;
    std::chrono::steady_clock::time_point startTime;

    
public:
    /**
     * Constructs a Circuit object.
     * Seed for the random number generator. If seed is -1, random number generator is created using the current time.
     * isTwoLevel can be specified to build tracks that have max 2 levels (0 and 1).
    */
    Circuit(std::vector<Piece> availablePieces, const int seed, const bool isTwoLevels);

    /**
     * Get the Json representation of this track.
    */
    std::string getTrackJson() const;
};
