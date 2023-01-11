#include "Vec2D.h"

void Vec2D::rotate(const Vec2D& rotationPoint, float angle) {
    // Displace this vector to rotate around the rotation point
    x -= rotationPoint.x;
    y -= rotationPoint.y;

    rotate(angle);

    // Re-translate around original center
    x += rotationPoint.x;
    y += rotationPoint.y;
}

void Vec2D::rotate(float angle) {
    // Avoid repetition of sin and cos calculations
    float cAngle = std::cos(angle);
    float sAngle = std::sin(angle);

    // Rotate point
    float tempa = cAngle * x - sAngle * y;
    y = sAngle * x + cAngle * y;
    x = tempa;
}
