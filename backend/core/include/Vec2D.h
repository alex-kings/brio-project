#pragma once
#include <cmath>

class Vec2D {
private:
    float a;
    float b;

public:

    Vec2D() = default;
    Vec2D(double x, double y) {
        a = x;
        b = y;
    }

    float getX() {return a;}
    float getY() {return b;}

    // Adds the given vector to this vector.
    void add(Vec2D v) {
        a += v.a;
        b += v.b;
    }

    // Dots this vector with the given vector.
    void dot(Vec2D v) {
        a *= v.a;
        b *= v.b;
    }

    // Return the modulus of this vector.
    float getModulus() {
        return std::sqrt(a*a + b*b);
    }
};
