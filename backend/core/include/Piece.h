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
    const std::string& getId() {
        return id;
    }

    // Parts
    Part& getPart(int index) {
        return parts[index];
    }
    int getNumberParts() {
        return parts.size();
    }

    // Connectors
    Connector& getConnector(int index) {
        return connectors[index];
    }
    int getNumberConnectors() {
        return connectors.size();
    }

};
