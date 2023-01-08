#include "Obb.h"
#include <iostream>

Obb::Obb(const Json::Value& jsonRep) {
    for(int a = 0; a < 4; a++) {
        points[a] = Vec2D(jsonRep[a]["x"].asFloat(), jsonRep[a]["y"].asFloat());
    }
}

void Obb::SATtest(const Vec2D& axis, const std::array<Vec2D, 4>& points, float& min, float& max) const {
    // Huge values.
    min = 100000000000000000000000000000000000.0, max = -100000000000000000000000000000000000.0;
    for(const Vec2D& point : points) {
        float dotVal = axis*point;
        if(dotVal < min) min = dotVal;
        if(dotVal > max) max = dotVal; 
    }
}

bool Obb::collides(const Obb& obb) const {
    // Finds the normals of this OBB
    Vec2D n1 = points[1] - points[0];
    Vec2D n2 = points[2] - points[1];

    // Find min and maxs for this OBB
    float thisMin1 = points[0] * n1;
    float thisMax1 = points[1] * n1;
    float thisMin2 = points[1] * n2;
    float thisMax2 = points[2] * n2;

    // Find min and maxs for the other obb
    float obbMin1, obbMax1, obbMin2, obbMax2;
    SATtest(n1, obb.points, obbMin1, obbMax1);
    SATtest(n2, obb.points, obbMin2, obbMax2);

    // Check overlaps on this OBB's axes
    if(!(overlap(thisMin1, thisMax1, obbMin1, obbMax1) && overlap(thisMin2, thisMax2, obbMin2, obbMax2))) {
        return false;
    };

    // Check the other figure's axes too, reusing the same variables

    // Finds the normals of the OBB
    n1 = obb.points[1] - obb.points[0];
    n2 = obb.points[2] - obb.points[1];

    // Find min and maxs for the OBB
    obbMin1 = obb.points[0] * n1;
    obbMax1 = obb.points[1] * n1;
    obbMin2 = obb.points[1] * n2;
    obbMax2 = obb.points[2] * n2;

    // Find min and maxs for this obb
    SATtest(n1, points, thisMin1, thisMax1);
    SATtest(n2, points, thisMin2, thisMax2);

    // Check for overlaps
    return(overlap(thisMin1, thisMax1, obbMin1, obbMax1) && overlap(thisMin2, thisMax2, obbMin2, obbMax2));
}


void Obb::rotate(const Vec2D& rotationPoint, float angle) {
    // Rotate each point.
    for(Vec2D& point : points) {
        point.rotate(rotationPoint, angle);
    }
}

void Obb::translate(const Vec2D& t) {
    for(Vec2D& point : points) {
        point.add(t);
    }
}

void Obb::translate(int x, int y) {
    for(Vec2D& point : points) {
        point.add(x, y);
    }
}

bool Obb::isBetween(float test, float a, float b) const {
    return ((test > a && test < b) || (test < a && test > b));
}

bool Obb::segmentIntersect(const Vec2D& a1, const Vec2D& a2, const Vec2D& b1, const Vec2D& b2) const {
    // Line 1 is horizontal
    if(a1.getX() == a2.getX()) {
        return isBetween(a1.getX(), b1.getX(), b2.getX());
    }

    // Line 2 is horizontal
    if(b1.getX() == b2.getX()) {
        return isBetween(b1.getX(), a1.getX(), a2.getX());
    }

    // No horizontal lines: can calculate the slopes
    float s1 = (a1.getY() - a2.getY()) / (a1.getX() - a2.getX());
    float s2 = (b1.getY() - b2.getY()) / (b1.getX() - b2.getX());

    if(s1 == s2) return false; // The two lines are collinear

    float k1 = a1.getY() - s1 * a1.getX();
    float k2 = b1.getY() - s2 * b1.getX();

    float xIntersect = (k2 - k1) / (s1 - s2);

    // Check intersection
    return (isBetween(xIntersect, a1.getX(), a2.getX()) && isBetween(xIntersect, b1.getX(), b2.getX()));
}

bool Obb::intersects(const Vec2D& p1, const Vec2D& p2) const {
    // Check whether the given segment intersects with any of this OBB's segments
    if(segmentIntersect(p1, p2, this->points[1], this->points[2])) return true;
    if(segmentIntersect(p1, p2, this->points[2], this->points[3])) return true;
    if(segmentIntersect(p1, p2, this->points[3], this->points[4])) return true;
    if(segmentIntersect(p1, p2, this->points[4], this->points[1])) return true;
    return false;
}
