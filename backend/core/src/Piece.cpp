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

    // Create ID
    id = pieceRep["id"].asString();
}