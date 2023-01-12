#pragma once
#include <json/json.h>
#include <vector>
#include "Obb.h"
#include <string>

/**
 * Part of a piece with a specific level and a set of OBBs.
 */

class Part
{

private:
    std::vector<Obb> obbs;
    // std::vector<Vec2D> bezierPoints;
    int level;

public:
    // Default constructor
    Part() {
        // bezierPoints = {};
        obbs = {};
        level = 0;
    }

    // Initialise from json representation
    Part(const Json::Value &jsonRep);

    // Parse to json
    std::string toJson() const {
        std::string jsonRep = "{\"rectangles\":[";

        // print obbs
        for (unsigned int i = 0; i < obbs.size(); i++) {
            jsonRep.append(obbs[i].toJson());
            if (i != obbs.size() - 1)
                jsonRep.append(",");
        }

        jsonRep.append("],\"level\":");
        jsonRep.append(std::to_string(level));
        // jsonRep.append(",\"bezierPoints\":[");

        // // print bezier points
        // for (unsigned int i = 0; i < bezierPoints.size(); i++) {
        //     jsonRep.append(bezierPoints[i].toJson());
        //     if (i < bezierPoints.size() - 1) jsonRep.append(",");
        // }

        // jsonRep.append("]}");
        jsonRep.append("}");

        return jsonRep;
    }

    const unsigned int getLevel() const {
        return this->level;
    }

    // Translate
    void translate(const Vec2D& amount) {
        // Obbs
        for(Obb& obb : obbs) {
            obb.translate(amount);
        }

        // // Bezier points
        // for(Vec2D& point : bezierPoints){
        //     point.add(amount);
        // }
    }

    // Rotate
    void rotate(const Vec2D& rotationPoint, float angle) {
        // rotate obbs
        for (Obb &obb : obbs) {
            obb.rotate(rotationPoint, angle);
        }

        // // rotate bezier points
        // for (Vec2D &p : bezierPoints) {
        //     p.rotate(rotationPoint, angle);
        // }
    }

    // Getters
    Obb &getObb(int index) {
        return obbs.at(index);
    }
    unsigned int getNumberObbs() {
        return obbs.size();
    }

    // Check collision between this part and another given part
    bool collides(const Part& part) const;

    // Check whether the given line passes through this part.
    bool intersects(const Vec2D& p1, const Vec2D& p2) const;

    // Changes the level by the given amount. It is trusted that the level will not be bought below 0.
    void changeLevel(const int amount);
};
