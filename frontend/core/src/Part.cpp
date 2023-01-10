#include "Part.h"

Part::Part(const Json::Value& jsonRep) {
    // Add obbs
    for(const Json::Value& rectangle : jsonRep["rectangles"]) {
        obbs.emplace_back(rectangle);
    }

    // // Add bezier points
    // for(const Json::Value& bezierPoint : jsonRep["bezierPoints"]) {
    //     bezierPoints.emplace_back(bezierPoint["x"].asFloat(), bezierPoint["y"].asFloat());
    // }

    // Add level
    level = jsonRep["level"].asInt();
}

bool Part::intersects(const Vec2D& p1, const Vec2D&p2) const {
    for(const Obb& o : this->obbs) {
        if(o.intersects(p1,p2)) return true;
    }
    return false;
}
