#pragma once
#include <json/json.h>
#include <vector>
#include "Obb.h"
#include <string>

/**
 * Part of a piece with a specific level and a set of OBBs.
*/

class Part {

private:
    std::vector<Obb> obbs;
    std::vector<Vec2D> bezierPoints;
    uint level;


public:
    // Default constructor
    Part() {
        bezierPoints = {};
        obbs = {};
        level = 0;
    }

    // Initialise from json representation
    Part(const Json::Value& jsonRep);

    // Parse to json
    std::string toJson() const {
        std::string jsonRep = "{\"rectangles\":[";

        // print obbs
        for(uint i = 0; i < obbs.size(); i++) {
            jsonRep.append(obbs[i].toJson());
            if(i != obbs.size() - 1) jsonRep.append(",");
        }

        jsonRep.append("],\"level\":");
        jsonRep.append(std::to_string(level));
        jsonRep.append(",\"bezierPoints\":[");

        // print bezier points
        for(uint i = 0; i < bezierPoints.size(); i++) {
            jsonRep.append(bezierPoints[i].toJson());
            if(i < bezierPoints.size() - 1) jsonRep.append(",");
        }

        jsonRep.append("]}");

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
