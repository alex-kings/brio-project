#include "Part.h"

Part::Part(const Json::Value& jsonRep) {
    // Add obbs
    for(const Json::Value& rectangle : jsonRep["rectanlges"]) {
        obbs.emplace_back(Obb(rectangle));
    }

    // Add level
    level = jsonRep["level"].asInt();
}