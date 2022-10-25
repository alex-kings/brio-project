#pragma once
#include "Vec2D.h"

/**
 * An oriented bounded box, ie. a rectangle used to compute collisions of track pieces.
*/

class Obb {

private:
    Vec2D p1;
    Vec2D p2;
    Vec2D p3;
    Vec2D p4;

public:
    Obb() = default;
    Obb(Vec2D p_1, Vec2D p_2, Vec2D p_3, Vec2D p_4) {
        p1 = p_1;
        p2 = p_2;
        p3 = p_3;
        p4 = p_4;
    };
};
