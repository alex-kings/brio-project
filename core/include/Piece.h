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
    float maxLength; // The maximum length between this piece's connectors.

    // Bounding circle
    float circleX;
    float circleY;
    float circleR;
    Vec2D circleCentre;

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
        for(unsigned int i = 0; i < parts.size(); i++) {
            jsonRep.append(parts[i].toJson());
            if(i != parts.size() - 1) jsonRep.append(",");
        }

        // Bounding circle
        jsonRep.append("],\"circleX\":" + std::to_string(circleCentre.getX()) + ",\"circleY\":" + std::to_string(circleCentre.getY()));
        jsonRep.append(",\"radius\":" + std::to_string(circleR));

        // print connectors
        jsonRep.append(",\"connectors\":[");
        for(unsigned int i = 0; i < connectors.size(); i++) {
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
    const bool isUsed() const {
        return this->used;
    }
    bool isFlippable() const {
        // Any flip type over 0 is flippable by definition.
        return (flipType > 0);
    }
    float getMaxLength() const {
        return maxLength;
    }

    // Setter
    void setUsed(bool u) {
        this->used = u;
        if(!u) {
            // Mark this piece's connectors as free
            for(Connector& con : connectors) {
                con.setConnected(false);
            }
        }
    }

    // Parts
    Part& getPart(int index) {
        return parts.at(index);
    }
    unsigned int getNumberParts() const {
        return parts.size();
    }

    // Connectors
    Connector& getConnector(int index) {
        return connectors.at(index);
    }
    unsigned int getNumberConnectors() const {
        return connectors.size();
    }

    // Returns the number of open connectors this piece has
    int openConNumber() const {
        int n = 0;    
        for(const Connector& con : this->connectors) {
            if(con.isFree()) n++;
        }
        return n;
    }

    /**
     * Gives this piece's open connectors.
    */
    std::vector<Connector*> getOpenConnectors() {
        std::vector<Connector*> openConnectors;
        for(int i = 0; i < this->connectors.size(); i++) {
            if(this->connectors.at(i).isFree()) openConnectors.push_back(&this->connectors.at(i));
        }   
        return openConnectors;
    }

    // Gives this piece's open connector
    Connector& getOpenConnector() {
        for(Connector& con : connectors) {
            if(con.isFree()) return con;
        }
        throw std::invalid_argument("This piece does not have any open connectors.");
    }

    // Tells whether this piece has open connectors
    bool hasOpenConnector() const {
        for(const Connector& con : connectors) {
            if(con.isFree()) return true; 
        }
        return false;
    }

    /**
     * Closes this piece's connectors
    */
    void closeConnectors() {
        for(Connector& con : this->connectors) {
            con.setConnected(false);
        }
    }

    // Translate this piece by the given amount
    void translate(const Vec2D& amount) {
        for(Part& part : parts) {
            part.translate(amount);
        }
        for(Connector& con : connectors) {
            con.translate(amount);
        }
        // Translate the bounding circle
        circleCentre.add(amount);
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

        // Rotate the bounding circle centre.
        circleCentre.rotate(rotationPoint, angle);
    }

    // Check whether this piece collides with the specified piece.
    bool collides(const Piece& piece) const;

    // Flips this piece
    void flip();

    // Returns the euclidian distance between the two first connectors of this piece
    float getDist() const {
        return connectors[0].getPosition().euclidianDist(connectors[1].getPosition());
    }

    // Place piece at origin
    void placeAtOrigin() {
        Vec2D currentPos = connectors[0].getPosition();
        this->translate(Vec2D(-currentPos.getX(), -currentPos.getY()));
    }

    // Checks whether the given line intersects with an OBB of this piece
    bool intersects(const Vec2D& p1, const Vec2D& p2) const;

    // Change the level of this piece by the given amount
    void changeLevel(const int amount);

    // Checks that this piece's lowest level is not below zero
    bool levelBelowZero() const;

    // Returns this piece's lowest level
    int getLowestLevel() const;
};
