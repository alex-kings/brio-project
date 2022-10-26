#include "Piece.h"

Piece::Piece(const Json::Value& pieceRep) {
    // Create Parts
    Json::Value partsRep = pieceRep["parts"];
    for(const Json::Value& partRep : partsRep) {
        parts.push_back(Part(partRep));
    }

    // Create Connectors
    Json::Value connectorsRep = pieceRep["connectors"];
    for(const Json::Value& connectorRep : connectorsRep) {
        connectors.push_back(Connector(connectorRep));
    }
}