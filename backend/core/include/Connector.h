#pragma once
#include "Vec2D.h"
#include <json/json.h>

enum class ConnectorType {in, out};

class Connector {
private:
    Vec2D position;
    Vec2D direction;
    ConnectorType type; // true = out, false = in.
    int level;
    

public:
    // Default constructor
    Connector();

    // Initialise from a json representation.
    Connector(const Json::Value& jsonRep);
};
