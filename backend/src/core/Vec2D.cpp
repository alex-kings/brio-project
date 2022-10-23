#include "Vec2D.h"

/**
 * 2D vector class.
*/

Vec2D::Vec2D(double a, double b) {
    x = a;
    y = b;
}

void Vec2D::add(Vec2D v){
    x += v.x;
    y += v.y;
}
