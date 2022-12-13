#include "Piece.h"

Piece::Piece(const Json::Value& pieceRep) {
    // Create Parts
    for(const Json::Value& partRep : pieceRep["parts"]) {
        parts.emplace_back(partRep);
    }

    // Create Connectors
    for(const Json::Value& connectorRep : pieceRep["connectors"]) {
        connectors.emplace_back(connectorRep);
    }

    // Create bounding circle
    circleX = pieceRep["boundingCircle"]["x"].asFloat();
    circleY = pieceRep["boundingCircle"]["y"].asFloat();
    circleR = pieceRep["boundingCircle"]["radius"].asFloat();

    // Create ID
    id = pieceRep["id"].asString();

    // Create flip
    flipType = pieceRep["flip"].asInt();
}

bool Piece::collides(const Piece& piece) const {
    // No collision if the pieces' circles don't collide
    float distSquared = (circleX - piece.circleX) * (circleX - piece.circleX) + (circleY - piece.circleY) * (circleY - piece.circleY);
    float distRadSquared = (circleR + piece.circleR) * (circleR + piece.circleR);
    if(distRadSquared < distSquared) return false;

    // Check collision between all of this and the other piece's parts.
    for(const Part& thisPart : parts) {
        for(const Part& piecePart : piece.parts) {
            if(thisPart.collides(piecePart)) return true;
        }
    }
    return false;
}