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
    // Default constructor
    Piece() {
        id = "";
        parts = {};
        connectors = {};
    }

    // Initialise from a json representation.
    Piece(const Json::Value& jsonRep);

    // Getters
    std::string getId() const {
        return id;
    }
    std::vector<Part> getParts() const {
        return parts;
    }
    std::vector<Connector> getConnectors() const {
        return connectors;
    }
};
