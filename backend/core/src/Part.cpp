#include "Part.h"

Part::Part(const Json::Value& jsonRep) {
    for(const Json::Value& rectangle : jsonRep["rectanlges"]) {
        obbs.push_back(Obb(rectangle));
    }
}