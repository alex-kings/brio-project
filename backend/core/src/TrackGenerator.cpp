#include "TrackGenerator.h"

#include <iostream>

Piece generateTrack(const Json::Value& selection) {
    // Get piece ressources
    Json::Value ressource = getPieceRessources();

    // Pieces available
    Json::Value::Members availablePieces = selection.getMemberNames();

    // Construct first piece from Json representation
    Json::Value firstPiece = ressource[availablePieces[0]];
    Piece* piece = new Piece(firstPiece);

    std::cout <<"Piece constructed. Id:"<< (*piece).getId() << std::endl;
    Part firstPart = (*piece).getParts()[0];

    std::cout << "Level of first part:" << firstPart.getLevel() << "\n";

    Obb firstObb = firstPart.getObbs()[0];

    std::cout <<"First point y"<< firstObb.getPoints()[0].getY() << "\n";
    
    

    return Piece();
}