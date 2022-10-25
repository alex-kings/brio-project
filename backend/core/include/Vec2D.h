#pragma once
#include <cmath>

class Vec2D {
private:
    float a;
    float b;

public:

    Vec2D() {a = b = 0;}

    Vec2D(float x, float y) {
        a = x;
        b = y;
    }

    float getX() {return a;}
    float getY() {return b;}

    // Adds the given vector to this vector.
    void add(const Vec2D& v) {
        a += v.a;
        b += v.b;
    }

    // Dots this vector with the given vector.
    void dot(const Vec2D& v) {
        a *= v.a;
        b *= v.b;
    }

    // Return the modulus of this vector.
    float getModulus() {
        return std::sqrt(a*a + b*b);
    }

    // Rotates this point around the given rotation point.
    void rotate(const Vec2D& rotationPoint, float angle) {
        float cAngle = std::cos(angle);
        float sAngle = std::sin(angle);

        float tempa = cAngle * rotationPoint.a + sAngle * rotationPoint.b;
        b = -sAngle * rotationPoint.a + cAngle * rotationPoint.b;
        a = tempa;
    }
};
