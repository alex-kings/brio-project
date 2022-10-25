#include "Obb.h"

void Obb::rotate(const Vec2D& rotationPoint, float angle) {
    // Rotate each point separately
    p1.rotate(rotationPoint, angle);
    p2.rotate(rotationPoint, angle);
    p3.rotate(rotationPoint, angle);
    p4.rotate(rotationPoint, angle);
}
