#include "json/json.h"
#include <iostream>
#include "ParseJson.h"
#include "TrackGenerator.h"
#include "Piece.h"
#include <vector>
#include "Vec2D.h"
#include "Track.h"

#include <thread>

#include <fstream>


void printHello(int i) {
    std::cout << "Hello" << i << "\n";
    std::cout << "Hello" << i << "\n";
    std::cout << "Hello" << i << "\n";
    std::cout << "Hello" << i << "\n";
}




int main(int, char* argv[]) {

    std::cerr << "Start" << "\n";

    // Get input from node program
    // std::string input = argv[1];


    // TEST
    std::string input = "{\"E\":\"8\",\"A\":\"6\",\"E1\":\"4\",\"D\":\"2\"}";


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

    // Get track
    // Piece startPiece = getTrack(pieceSelection);

    Track t(pieces);


    // TEST MULTITHREADING

    // uint maxThreads = std::thread::hardware_concurrency()*2; // 2 times the number of available cores.
    // std::thread threads[maxThreads];
    // for (uint i = 0; i < maxThreads; i++) {
    //     threads[i] = std::thread(printHello, i);
    // }
    // for(uint i = 0; i < maxThreads; i++) {
    //     threads[i].join();
    // }
}




