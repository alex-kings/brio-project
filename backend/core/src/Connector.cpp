#include "Connector.h"


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
    type = (jsonRep["type"].asBool()) ? ConnectorType::out : ConnectorType::in;
    level = jsonRep["level"].asInt();
}