#ifndef VEC2D_H
#define VEC2D_H

class Vec2D {
private:
    float x;
    float y;
public:
    Vec2D(double x, double y);
    void add(Vec2D v);
};

#endif