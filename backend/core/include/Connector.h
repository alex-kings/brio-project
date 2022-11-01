#pragma once
#include "Vec2D.h"
#include <json/json.h>
#include <iostream>


class Connector {
private:
    Vec2D position;
    Vec2D direction;
    bool type; // true = in, false = out.
    int level;
    Connector* connection = nullptr; // Connector linked to this connector, initially nullptr.
    
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

    // Link this Connector to another Connector.
    void link(Connector* c) {
        connection = c;
    }

    // Getters
    Connector* getConnection() const {
        return connection;
    }
    bool getType() const {
        return type;
    }
    
    // Tells if this connector is free.
    bool isFree() const {
        return (connection == nullptr);
    }
};
