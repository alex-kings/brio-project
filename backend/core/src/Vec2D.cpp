#include "Vec2D.h"

Vec2D::Vec2D() {a = b = 0;}

Vec2D::Vec2D(float x, float y) {
    a = x;
    b = y;
}

void Vec2D::add(const Vec2D& v) {
    a += v.a;
    b += v.b;
}

void Vec2D::add(int x, int y) {
    a += x;
    b += y;
}

void Vec2D::dot(const Vec2D& v) {
    a *= v.a;
    b *= v.b;
}

float Vec2D::getModulus() {
    return std::sqrt(a*a + b*b);
}

// Rotates this point around the given rotation point.
void Vec2D::rotate(const Vec2D& rotationPoint, float angle) {
    // Displace this vector to rotate around the rotation point
    a -= rotationPoint.a;
    b -= rotationPoint.b;

    // Avoid repetition of sin and cos calculations
    float cAngle = std::cos(angle);
    float sAngle = std::sin(angle);

    // Rotate point
    float tempa = cAngle * a + sAngle * b;
    b = - sAngle * a + cAngle * b;
    a = tempa;

    // Re-translate around original center
    a += rotationPoint.a;
    b += rotationPoint.b;
}
