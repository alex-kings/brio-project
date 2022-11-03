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
    bool used = false;  // Tells whether the piece is in use.

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
    const std::string& getId() const {
        return id;
    }

    bool isUsed() const {
        return used;
    }

    // Setter
    void setUsed(bool u) {
        used  = u;
    }

    // Parts
    Part& getPart(int index) {
        return parts.at(index);
    }
    uint getNumberParts() const {
        return parts.size();
    }

    // Connectors
    Connector& getConnector(int index) {
        return connectors.at(index);
    }
    uint getNumberConnectors() const {
        return connectors.size();
    }

    // Rotate piece around the given point. This is done by rotating all the obbs and the connectors of this piece around the given rotation point.
    void rotate(const Vec2D& rotationPoint, float angle) {
        // Rotate OBBs
        for(Part& part : parts) {
            for(uint i = 0; i < part.getNumberObbs(); i++) {
                part.getObb(i).rotate(rotationPoint, angle);
            }
        }

        // Rotate connector positions and directions
        for(Connector& con : connectors) {
            con.rotate(rotationPoint, angle);
        }
    }

};
