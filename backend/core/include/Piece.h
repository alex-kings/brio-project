#pragma once
#include "Part.h"
#include "Connector.h"
#include <vector>
#include <string>
#include <json/json.h>

/**
 * A track piece.
*/

class Piece {
private:
    std::string id;
    std::vector<Part> parts;
    std::vector<Connector> connectors;
public:
    Piece() {
        id = "";
        parts = {};
        connectors = {};
    }

    // Initialise from a json representation.
    Piece(const Json::Value& jsonRep);
};
