#pragma once
#include "Vec2D.h"
#include <json/json.h>

/**
 * An oriented bounded box, ie. a rectangle used to compute collisions of track pieces.
*/

class Obb {

private:
    std::array<Vec2D, 4> points;

public:
    // Default constructor
    Obb() {
        points = {Vec2D(), Vec2D(), Vec2D(), Vec2D()};
    }

    // Initialise from json representation.
    Obb(const Json::Value& jsonRep);

    // Getter
    std::array<Vec2D, 4> getPoints() const {
        return points;
    }

    // Rotate each point of this OBB around the given point
    void rotate(const Vec2D& rotationPoint, float angle);

    // Translate each point of this OBB by a given amount
    void translate(const Vec2D& t);

    // Translate each point of this OBB by a given x and y
    void translate(int x, int y);
};
