#include "Obb.h"

Obb::Obb(const Json::Value& jsonRep) {
    for(int a = 0; a < 4; a++) {
        points[a] = Vec2D(jsonRep[a]["x"].asFloat(), jsonRep[a]["y"].asFloat());
    }
}

void Obb::rotate(const Vec2D& rotationPoint, float angle) {
    // Rotate each point.
    for(Vec2D& point : points) {
        point.rotate(rotationPoint, angle);
    }
}

void Obb::translate(const Vec2D& t) {
    for(Vec2D& point : points) {
        point.add(t);
    }
}

void Obb::translate(int x, int y) {
    for(Vec2D& point : points) {
        point.add(x, y);
    }
}
