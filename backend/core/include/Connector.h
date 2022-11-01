#pragma once
#include "Vec2D.h"
#include <json/json.h>
#include <iostream>

enum class ConnectorType {in, out};

class Connector {
private:
    Vec2D position;
    Vec2D direction;
    ConnectorType type;
    int level;
    Connector* connection = nullptr; // Connector linked to this connector, initially nullptr.
    
public:
    // Default constructor
    Connector() {
        position = Vec2D();
        direction = Vec2D();
        type = ConnectorType::out;
        level = 0;
    }

    // Initialise from a json representation.
    Connector(const Json::Value& jsonRep);

    // Link this Connector to another Connector.
    void link(Connector* c) {
        connection = c;
    }

    Connector* getConnection() {
        return connection;
    }
    
    // Tells if this connector is free.
    bool isFree() {
        return (connection == nullptr);
    }
};
