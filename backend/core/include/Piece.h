#pragma once
#include "Part.h"
#include "Connector.h"
#include <vector>
#include <string>
#include <json/json.h>
#include <stdexcept>

/**
 * A track piece.
*/

class Piece {

private:
    std::string id;
    std::vector<Part> parts;
    std::vector<Connector> connectors;
    bool used = false;  // Tells whether the piece is in use.
    int flipType = 0; // This tells how to best flip this piece.

public:
    // Default constructor
    Piece() {
        id = "";
        parts = {};
        connectors = {};
    }

    // Initialise from a json representation.
    Piece(const Json::Value& jsonRep);

    // Return a json representation of this
    std::string toJson() const {
        std::string jsonRep = "{\"id\":\"" + id + "\",\"used\":" + (used ? "true" : "false") + ",\"parts\":[";
        
        // print parts
        for(uint i = 0; i < parts.size(); i++) {
            jsonRep.append(parts[i].toJson());
            if(i != parts.size() - 1) jsonRep.append(",");
        }

        jsonRep.append("],\"connectors\":[");

        // print connectors
        for(uint i = 0; i < connectors.size(); i++) {
            jsonRep.append(connectors[i].toJson());
            if(i != connectors.size() - 1) jsonRep.append(",");
        }

        jsonRep.append("]}");

        return jsonRep;
    }

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

    // Tells whether this piece has open connectors
    bool hasOpenConnectors() const {
        for(const Connector& con : connectors) {
            if(con.isFree()) return true;
        }
        return false;
    }

    // Gives this piece's open connector
    const Connector& getOpenConnector() const {
        for(const Connector& con : connectors) {
            if(con.isFree()) return con;
        }
        throw std::invalid_argument("This piece does not have any open connectors.");
    }

    // Translate this piece by the given amount
    void translate(const Vec2D& amount) {
        for(Part& part : parts) {
            part.translate(amount);
        }
        for(Connector& con : connectors) {
            con.translate(amount);
        }
    }

    // Rotate piece around the given point. This is done by rotating all the obbs and the connectors of this piece around the given rotation point.
    void rotate(const Vec2D rotationPoint, float angle) {
        // Rotate parts
        for(Part& part : parts) {
            part.rotate(rotationPoint, angle);
        }

        // Rotate connector positions and directions
        for(Connector& con : connectors) {
            con.rotate(rotationPoint, angle);
        }
    }

    // Check whether this piece collides with the specified piece.
    bool collides(const Piece& piece) const {
        // Check collision between all of this and the other piece's parts.
        for(const Part& thisPart : parts) {
            for(const Part& piecePart : piece.parts) {
                if(thisPart.collides(piecePart)) return true;
            }
        }
        return false;
    }

    // Flips this piece
    void flip() {
        if(flipType == 0) return; // No need to do anything.
        if(flipType == 1) { // Switch the two connectors (there has to be two for this switch type).
            // Assumes that the connectors have to be of different types for this to work.
            connectors.at(0).switchType();
            connectors.at(1).switchType();
            return;
        }
        // Have to perform a full flip of the piece
    }
};
