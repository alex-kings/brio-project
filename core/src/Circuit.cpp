#include "Circuit.h"
#include "Loop.h"
#include <string>
#include <iostream> 

Circuit::Circuit(std::vector<Piece> availablePieces, const int seed, const bool isTwoLevel) {

    // Start timer for the entire generation.
    this->startTime = std::chrono::steady_clock::now();
    this->isTwoLevel = isTwoLevel;
    this->pieces = availablePieces;

    // Generate random engine with seed. If seed is -1, the current time is used for the seed.
    if(seed == -1) this->randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    else this->randomEngine = std::default_random_engine(seed);
}

std::string Circuit::generate() {
    // Generate the track using a Loop object.
    Loop l(pieces, 0, pieces.size(), &pieces[0], &pieces[0], this->randomEngine, this->isTwoLevel);
    l.generate();
    return l.getTrackJson();
}

std::string Circuit::getTrackJson() const {
    std::string result;
    result += "{\"pieces\":[";
    for(unsigned int i = 0; i < pieces.size(); i++) {
        result += pieces[i].toJson();
        if(i < pieces.size() - 1) result += ",";
    }
    result += "]}";
    return result;
}