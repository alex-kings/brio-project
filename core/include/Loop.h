#pragma once
#include "Piece.h"
#include <vector>
#include <random>

/**
 * Class representing a closed loop inside a Track.
*/

class Loop {

private:
    std::vector<Piece> availablePieces;
    std::vector<Piece> placedPieces;
    std::default_random_engine randomEngine;
    unsigned int generationCount = 0;
    const unsigned int maxGenerations = 100; // 100
    const unsigned int maxNumberRecursions = 10000; // 10 000
    unsigned int currentNumberRecursions = 0; 
    unsigned int nbPiecesPlaced = 0;
    int maxLevel;



public:
    /**
     * Construct a Loop object from a set of pieces and a random number generator.
    */
    Loop(std::vector<Piece> available, std::vector<Piece> placed, std::default_random_engine engine, bool isTwoLevel);
};

