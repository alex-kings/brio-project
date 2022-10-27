#pragma once
#include <json/json.h>
#include <vector>
#include "Obb.h"

/**
 * Part of a piece with a specific level and a set of OBBs.
*/

class Part {

private:
    std::vector<Obb> obbs;
    int level;

public:
    // Default constructor
    Part();

    // Initialise from json representation
    Part(const Json::Value& jsonRep);
};

Part::Part() {
    obbs = {};
    level = 0;
}
