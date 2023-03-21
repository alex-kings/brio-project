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
    void SATtest(const Vec2D& axis, const std::array<Vec2D, 4>& points, float& min, float& max) const;

    // Tells whether there is an overlap between the two given sets of floats.
    bool overlap(float min1, float max1, float min2, float max2) const {
        return((min2 >= min1 && min2 <= max1) || (min1 >= min2 && min1 <= max2));
    }

    // Determines whether the two segments a and b intersect each other.
    bool segmentIntersect(const Vec2D& a1, const Vec2D& a2, const Vec2D& b1, const Vec2D& b2) const;

    // Determines whether the given float is between the two next floats
    bool isBetween(float test, float a, float b) const;

public:
    // Default constructor
    Obb() {
        points = {Vec2D(), Vec2D(), Vec2D(), Vec2D()};
    }

    // Initialise from json representation.
    Obb(const Json::Value& jsonRep);

    // Construct between the two given points
    Obb(const Vec2D& v1, const Vec2D& v2);

    // parse to json
    std::string toJson() const {
        std::string jsonRep = "[";

        for(unsigned int i = 0; i < points.size(); i++) {
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
    bool collides(const Obb& obb) const;

    // Rotate each point of this OBB around the given point
    void rotate(const Vec2D& rotationPoint, float angle);

    // Translate each point of this OBB by a given amount
    void translate(const Vec2D& t);

    // Translate each point of this OBB by a given x and y
    void translate(int x, int y);

    // Checks whether the line between the two given points intersects this OBB
    bool intersects(const Vec2D& p1, const Vec2D& p2) const;
};
