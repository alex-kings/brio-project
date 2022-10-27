#include "Piece.h"

Piece::Piece() {
    id = "";
    parts = {};
    connectors = {};
}

Piece::Piece(const Json::Value& pieceRep) {
    // Create Parts
    for(const Json::Value& partRep : pieceRep["parts"]) {
        parts.push_back(Part(partRep));
    }

    // Create Connectors
    for(const Json::Value& connectorRep : pieceRep["connectors"]) {
        connectors.push_back(Connector(connectorRep));
    }

    // Create ID
    id = pieceRep["id"].asString();
}