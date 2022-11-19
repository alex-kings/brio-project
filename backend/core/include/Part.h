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
    uint level;


public:
    // Default constructor
    Part() {
        obbs = {};
        level = 0;
    }

    // Initialise from json representation
    Part(const Json::Value& jsonRep);

    // Parse to json
    std::string toJson() const {
        std::string jsonRep = "{\"obbs\":[";

        // print obbs
        for(const Obb& obb : obbs) {
            jsonRep.append(obb.toJson());
        }

        jsonRep.append("],\"level\":");
        jsonRep.append(std::to_string(level));
        jsonRep.append("}");

        return jsonRep;
    }

    //Getters
    Obb& getObb(int index) {
        return obbs.at(index);
    }
    uint getNumberObbs() {
        return obbs.size();
    }

    uint getLevel() const {
        return level;
    }
};
