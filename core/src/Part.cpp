#include "Part.h"

Part::Part(const Json::Value& jsonRep) {
    // Add obbs
    for(const Json::Value& rectangle : jsonRep["rectangles"]) {
        obbs.emplace_back(rectangle);
    }

    // Add level
    level = jsonRep["level"].asInt();
}

bool Part::collides(const Part& part) const {
    // If the levels aren't the same, the parts don't collide.
    if(this->level != part.level) return false;
    
    // Checks collision between each Obb of each part.
    for(const Obb& thisObb : obbs) {
        for(const Obb& partObb : part.obbs) {
            if(thisObb.collides(partObb)) return true;
        }
    }
    return false;
}

bool Part::intersects(const Vec2D& p1, const Vec2D&p2) const {
    for(const Obb& o : this->obbs) {
        if(o.intersects(p1,p2)) return true;
    }
    return false;
}

void Part::changeLevel(const int amount) {
    this->level += amount;
}

