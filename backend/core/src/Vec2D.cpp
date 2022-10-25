#include "Vec2D.h"


// Rotates this point around the given rotation point.
// FIX ROTATION IS ONLY AROUND ORIGIN JUST NOW!!!
void Vec2D::rotate(const Vec2D& rotationPoint, float angle) {
    float cAngle = std::cos(angle);
    float sAngle = std::sin(angle);

    float tempa = cAngle * rotationPoint.a + sAngle * rotationPoint.b;
    b = -sAngle * rotationPoint.a + cAngle * rotationPoint.b;
    a = tempa;
}
