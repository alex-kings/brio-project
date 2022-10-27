#include "Obb.h"

Obb::Obb(const Json::Value& jsonRep) {
    
    for(int a = 0; a < 4; a++) {
        points[a] = Vec2D(jsonRep[a]["x"].asFloat(), jsonRep[a]["y"].asFloat());
    }
}

void Obb::rotate(const Vec2D& rotationPoint, float angle) {
    // Rotate each point.
    for(const Vec2D& point : points) {
        point.rotate(rotationPoint, angle);
    }
}
