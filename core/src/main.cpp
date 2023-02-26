#include <emscripten/bind.h>

#include "json/json.h"
#include <iostream>
#include "ParseJson.h"
#include "Piece.h"
#include <vector>
#include "Vec2D.h"
#include "Circuit.h"

#include <fstream>
#include <thread>
#include <ctime>

// using namespace emscripten;


void printHello(int i) {
    std::cout << "Hello" << i << "\n";
    std::cout << "Hello" << i << "\n";
    std::cout << "Hello" << i << "\n";
    std::cout << "Hello" << i << "\n";
}

int main() {
    // Test pieces.
    std::string input = "{ \"E\": \"12\" }";

    // User selection of pieces and their available quantities.
    Json::Value pieceSelection;
    try {
        // Example
        pieceSelection = readJson(input);
    }
    catch(const std::domain_error& e) {
        std::cerr << e.what() << std::endl;
    }

    Json::Value ressources = getPieceRessources(); // Library of pieces
    std::vector<Piece> pieces;
    for(const std::string& member : pieceSelection.getMemberNames()) {
        for(int i = 0; i < std::stoi(pieceSelection[member].asString()); i++) {
            // Add Piece to available pieces.
            pieces.emplace_back(ressources[member]);
        }
    }

    // Create output file
    std::ofstream f;
    std::time_t now = std::time(0);
    std::string dt = std::ctime(&now);
    f.open("timing_" + dt + ".csv");

    // Test building the track for seeds 1 to 100
    for(int i = 1; i < 101; i++) {
        std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

        // Create circuit ...
        Circuit track(pieces, i, false, "close");
        if(!track.generate()) {
            // Couldn't generate track
            f << "-1,";
            continue;
        }

        // Time of execution
        std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        // Add execution time to the list.
        f << elapsed << ",";
    }

}

std::string generateTrack(const std::string& selection, const int seed, const int isTwoLevel, const std::string vCondition) {
    // User selection of pieces and their available quantities.
    // std::string input = "{\"E\":\"8\",\"A\":\"6\",\"E1\":\"4\",\"D\":\"2\"}";

    Json::Value pieceSelection;
    try {
        // Example
        pieceSelection = readJson(selection);
    }
    catch(const std::domain_error& e) {
        std::cerr << e.what() << std::endl;
    }

    Json::Value ressources = getPieceRessources(); // Library of pieces
    std::vector<Piece> pieces;
    for(const std::string& member : pieceSelection.getMemberNames()) {
        for(int i = 0; i < std::stoi(pieceSelection[member].asString()); i++) {
            // Add Piece to available pieces.
            pieces.emplace_back(ressources[member]);
        }
    }

    // Get track
    Circuit t(pieces, seed, isTwoLevel, vCondition);

    if(t.generate()) {
        return t.getTrackJson();
    }
    else return "{\"error\":1}";
}

/**
 * Expose generating function
*/
EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("generateTrack", &generateTrack);
}

