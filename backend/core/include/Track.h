#pragma once
#include <json/json.h>
#include <vector>
#include "Piece.h"


/** 
 * Class for the generation of tracks.
*/

class Track {

private:
    std::vector<Piece> pieces;

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

    /**
     * Recursively connects pieces together to generate a closed track.
     * The validation conditions need to be met for the track to be successfully built.
    */
    bool generateTrack(const Piece& lastPiece, Connector& openConnector);

    /**
     * Attempt placement of the given piece.
    */
    bool attemptPlacement(Piece& testPiece, const Piece& lastPiece, Connector& openConnector);

public:
    /**
     * Construct from a selection of pieces.
    */
    Track(const Json::Value& selection);

    void writeToFile() const;
};
