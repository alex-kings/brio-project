#pragma once
#include <string>
#include <cmath>
#include <iostream>

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

    // Parse to Json
    std::string toJson() const {
        return "{\"x\":" + std::to_string(x) + ",\"y\":" + std::to_string(y) + "}";
    }

    // Getters
    float getX() const {return x;}
    float getY() const {return y;}

    // Overload operator + for vector addition
    Vec2D operator + (const Vec2D& v) const {
        Vec2D res;
        res.x = x + v.x;
        res.y = y + v.y;
        return res;
    }

    // Overload operator - for vector subtraction
    Vec2D operator - (Vec2D const& v) const {
        return Vec2D(x-v.x, y-v.y);
    }

    // Scale this vector by the given float and return the result
    Vec2D scale(float n) {
        return Vec2D(x*n, y*n);
    }

    // Overload operator * for dot product
    float operator * (const Vec2D& v) const {
        return x*v.x + y*v.y;
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

    // returns the result of the dot product between this and the given vector
    float dot(const Vec2D& v) const {
        return (x * v.x + y * v.y);
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
        float val = (x*v.x + y*v.y) / (getModulus()*v.getModulus());
        
        // Safe acos.
        float theta;
        if(val <= -1.0) theta = M_PI;
        else if(val >= 1.0) theta = 0;
        else theta = acos(val);

        // Sign of theta
        if((x*v.y - y*v.x) < 0) {
            return (2*M_PI - theta);
        }
        return theta;
    }

    // Returns the euclidian distance between this and the given point
    float euclidianDist(const Vec2D& v) const {
        float dx = x - v.x;
        float dy = y - v.y;
        // Maybe can get rid of the square root for most uses?
        return std::sqrt(dx*dx + dy*dy);
    }

    // Returns the square of the euclidian distance between this and the given point.
    float euclidianDistSquared(const Vec2D& v) const {
        float dx = x - v.x;
        float dy = y - v.y;
        return dx*dx + dy*dy;
    }

    // Returns the absolute angle difference between two vectors
    float absoluteAngleDiff(const Vec2D& v) const {
        float val = dot(v)/(getModulus() * v.getModulus());
        if(val <= -1) return M_PI;
        if(val >= 1) return 0;
        return std::acos(val);
    }

    // Changes this point's coordinates to it's symmetrical position along the x axis.
    void horizontalFlip() {
        this->y = - this->y;
    }
};
