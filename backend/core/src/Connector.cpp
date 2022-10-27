#include "Connector.h"

Connector::Connector() {
    position = Vec2D();
    direction = Vec2D();
    type = true;
    level = 0;
}

Connector::Connector(const Json::Value& jsonRep) {
    // Get position
    float x = std::stof(jsonRep["pos"]["x"].asString());
    float y = std::stof(jsonRep["pos"]["y"].asString());
    position = Vec2D(x, y);

    // Get direction
    float dx = std::stof(jsonRep["direction"]["x"].asString());
    float dy = std::stof(jsonRep["direction"]["y"].asString());
    direction = Vec2D(dx, dy);

    // Get type and level
    type = jsonRep["type"].asBool();
    level = jsonRep["level"].asInt();
}