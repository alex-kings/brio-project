#include "Part.h"

Part::Part(const Json::Value& jsonRep) {
    // Add obbs
    for(const Json::Value& rectangle : jsonRep["rectangles"]) {
        obbs.emplace_back(rectangle);
    }

    // Add bezier points
    for(const Json::Value& bezierPoint : jsonRep["bezierPoints"]) {
        bezierPoints.emplace_back(bezierPoint["x"].asFloat(), bezierPoint["y"].asFloat());
    }

    // Add level
    level = jsonRep["level"].asInt();
}