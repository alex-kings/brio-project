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
    Obb(const Vec2D& p_1, const Vec2D& p_2, const Vec2D& p_3, const Vec2D& p_4) {
        p1 = p_1;
        p2 = p_2;
        p3 = p_3;
        p4 = p_4;
    };

    // Rotate each point of this OBB around the given point
    void rotate(const Vec2D& rotationPoint, float angle);
};
