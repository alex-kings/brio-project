#pragma once
#include "Vec2D.h"
#include <json/json.h>

class Connector {
private:
    Vec2D position;
    Vec2D direction;
    bool type; // true = out, false = in.
    int level;
    

public:
    Connector() {
        position = Vec2D();
        direction = Vec2D();
        type = true;
        level = 0;
    }

    // Initialise from a json representation.
    Connector(const Json::Value& jsonRep);
};
