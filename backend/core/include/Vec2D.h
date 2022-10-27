#pragma once
#include <cmath>

class Vec2D {
private:
    float a;
    float b;

public:
    // Constructors
    Vec2D();
    Vec2D(float x, float y);

    // Getters
    float getX() {return a;}
    float getY() {return b;}

    // Adds the given vector to this vector.
    void add(const Vec2D& v);

    // Add the given vector to this vector.
    void add(int x, int y);

    // Dots this vector with the given vector.
    void dot(const Vec2D& v);

    // Return the modulus of this vector.
    float getModulus();

    // Rotates this point around the given rotation point.
    void rotate(const Vec2D& rotationPoint, float angle);    
};
