#pragma once
#include "Vec2D.h"
#include <json/json.h>
#include <string>

/**
 * An oriented bounded box, ie. a rectangle used to compute collisions of track pieces.
*/

class Obb {

private:
    std::array<Vec2D, 4> points;

    // Gives the min and max of the points along the given axis.
    void SATtest(const Vec2D& axis, const std::array<Vec2D, 4>& points, float& min, float& max);

    // Tells whether there is an overlap between the two given sets of floats.
    bool overlap(float min1, float max1, float min2, float max2) {
        return((min2 >= min1 && min2 <= max1) || (min1 >= min2 && min1 <= max2));
    }

public:
    // Default constructor
    Obb() {
        points = {Vec2D(), Vec2D(), Vec2D(), Vec2D()};
    }

    // TEST CONSTRUCTOR
    Obb(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y) {
        points = {Vec2D(p1x, p1y),Vec2D(p2x, p2y),Vec2D(p3x, p3y),Vec2D(p4x, p4y)};
    }

    // Initialise from json representation.
    Obb(const Json::Value& jsonRep);

    // parse to json
    std::string toJson() const {
        std::string jsonRep = "[";

        for(uint i = 0; i < points.size(); i++) {
            jsonRep.append(points[i].toJson());
            if(i != points.size() - 1) jsonRep.append(",");
        }

        jsonRep.append("]");
        return jsonRep;
    }

    // Getter
    Vec2D& getPoint(int index) {
        return points[index];
    }

    // Check whether this OBB collides with the given one using SAT
    bool collides(const Obb& obb);

    // Rotate each point of this OBB around the given point
    void rotate(const Vec2D& rotationPoint, float angle);

    // Translate each point of this OBB by a given amount
    void translate(const Vec2D& t);

    // Translate each point of this OBB by a given x and y
    void translate(int x, int y);
};
