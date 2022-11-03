#pragma once
#include <cmath>

class Vec2D {
private:
    float x;
    float y;

public:
    // Constructors
    Vec2D() { x = y = 0; }
    Vec2D(float a, float b) {
        x = a;
        y = b;
    }

    // Getters
    float getX() const {return x;}
    float getY() const {return y;}

    // Overload operator + for vector addition
    Vec2D operator + (Vec2D const& v) {
        Vec2D res;
        res.x = x + v.x;
        res.y = y + v.y;
        return res;
    }

    // Overload operator * for dot product
    Vec2D operator * (Vec2D const& v) {
        Vec2D res;
        res.x = x * v.x;
        res.y = y * v.y;
        return res;
    }

    // Adds the given vector to this vector.
    void add(const Vec2D& v) {
        x += v.x;
        y += v.y;
    }

    // Add the given vector to this vector.
    void add(int a, int b) {
        x += a;
        y += b;
    }

    // Dots this vector with the given vector.
    void dot(const Vec2D& v) {
        x *= v.x;
        y *= v.y;
    }

    // Return the modulus of this vector.
    float getModulus() const { 
        return std::sqrt(x*x + y*y);
    }

    // Rotates this point counterclockwise around the given rotation point.
    void rotate(const Vec2D& rotationPoint, float angle);    

    // Rotates this point counterclockwise around the origin.
    void rotate(float angle);

    // Gives the angle difference between this and another Vec2D. Positive is counterclockwise. First vector is this.
    float getAngleDifference(const Vec2D& v) const {
        float theta = acos((x*v.x + y*v.y) / (getModulus()*v.getModulus()));
        // Sign of theta
        if((x*v.y - y*v.x) < 0) return (2*M_PI - theta);
        return theta;
    }
};
