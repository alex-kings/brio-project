#pragma once
#include "Vec2D.h"
#include <json/json.h>
#include <iostream>
#include <string>
#include <cmath>


class Connector {
private:
    Vec2D position;
    Vec2D direction;
    bool type; // true = in, false = out.
    uint level;
    Connector* connection = nullptr; // Connector linked to this connector, initially nullptr.
    bool connected = false; // Tells whether this connector is connected to another.
    
public:
    // Default constructor
    Connector() {
        position = Vec2D();
        direction = Vec2D();
        type = true;
        level = 0;
    }

    // Initialise from a json representation.
    Connector(const Json::Value& jsonRep);

    // parse to json
    std::string toJson() const {
        std::string jsonRep = "{\"position\":";
        jsonRep.append(position.toJson());
        jsonRep.append(",\"direction\":");
        jsonRep.append(direction.toJson());
        jsonRep.append(",\"type\":");
        jsonRep.append(type ? "true" : "false");
        jsonRep.append(",\"level\":");
        jsonRep.append(std::to_string(level));
        jsonRep.append("}");
        return jsonRep;
    }

    // Link this and another connector together.
    void link(Connector* c) {
        connection = c;
        c->connection = this;
    }

    // Unlink connection between this and the specified connector.
    void unlink(Connector* c) {
        connection = nullptr;
        c->connection = nullptr;
    }

    // Getters
    Connector* getConnection() const {
        return connection;
    }
    bool getType() const {
        return type;
    }
    const Vec2D& getPosition() const {
        return position;
    }
    const Vec2D& getDirection() const {
        return direction;
    }

    // Setters
    void setType(bool t) {
        type = t;
    }

    // Changes this connector's type
    void switchType() {
        type = !type;
    }

    // Tells if this connector is free.
    bool isFree() const {
        return (connection == nullptr);
    }

    // Translate this connector's position by the given amount
    void translate(const Vec2D& amount) {
        position.add(amount);
    }

    // Rotate this connector's position and direction around the given rotation point.
    void rotate(const Vec2D& rotationPoint, float angle) {
        // Rotate position around rotation point
        position.rotate(rotationPoint,angle);

        // Rotate direction around origin
        direction.rotate(angle);
    }

    // Checks whether the given validation conditions are met for the two connectors.
    bool validate(const Connector& con, const float validationAngle, const float validationDist) {
        // Only euclidian distance for now...
        return ( (position.euclidianDist(con.position) <= validationDist) );
        // (std::abs(direction.getAngleDifference(con.direction)) <= validationAngle) );
    }
};
