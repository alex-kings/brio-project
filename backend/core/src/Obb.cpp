#include "Obb.h"

Obb::Obb(const Json::Value& jsonRep) {
    // TODO!!
}

void Obb::rotate(const Vec2D& rotationPoint, float angle) {
    // Rotate each point.
    p1.rotate(rotationPoint, angle);
    p2.rotate(rotationPoint, angle);
    p3.rotate(rotationPoint, angle);
    p4.rotate(rotationPoint, angle);
}
