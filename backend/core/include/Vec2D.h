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

    // Add the given vector to this vector.
    void add(int x, int y) {
        a += x;
        b += y;
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
    void rotate(const Vec2D& rotationPoint, float angle);    
};
