#include "Piece.h"

Piece::Piece(const Json::Value& pieceRep) {
    // Create Parts
    for(const Json::Value& partRep : pieceRep["parts"]) {
        parts.emplace_back(partRep);
    }

    // Create Connectors
    for(const Json::Value& connectorRep : pieceRep["connectors"]) {
        connectors.emplace_back(connectorRep);
    }

    // Create bounding circle
    circleX = pieceRep["boundingCircle"]["x"].asFloat();
    circleY = pieceRep["boundingCircle"]["y"].asFloat();
    circleR = pieceRep["boundingCircle"]["radius"].asFloat();

    // Create ID
    id = pieceRep["id"].asString();

    // Create flip
    flipType = pieceRep["flip"].asInt();
}

bool Piece::collides(const Piece& piece) const {
    // No collision if the pieces' circles don't collide
    float distSquared = (circleX - piece.circleX) * (circleX - piece.circleX) + (circleY - piece.circleY) * (circleY - piece.circleY);
    float distRadSquared = (circleR + piece.circleR) * (circleR + piece.circleR);
    if(distRadSquared < distSquared) return false;

    // Check collision between all of this and the other piece's parts.
    for(const Part& thisPart : parts) {
        for(const Part& piecePart : piece.parts) {
            if(thisPart.collides(piecePart)) return true;
        }
    }
    return false;
}

bool Piece::intersects(const Vec2D& p1, const Vec2D& p2) const {
    for(const Part& part : this->parts) {
        if(part.intersects(p1, p2)) return true;
    }
    return false;
}

void Piece::flip() {
    if(flipType == 0) return; // No need to do anything.
    if(flipType == 1) { // Switch the two connectors (there has to be two for this switch type).
        // Assumes that the connectors have to be of different types for this to work.
        connectors.at(0).switchType();
        connectors.at(1).switchType();
        return;
    }
    if(flipType == 2) { // Have to perform a full flip of the piece
        // The flip happens along the x axis, using a flipping matrix ((1,0),(0,-1))
        
    }
}


void Piece::changeLevel(const int amount) {
    // Modify level
    for(Connector& con : this->connectors) con.changeLevel(amount);
    for(Part& part : this->parts) part.changeLevel(amount);
}

bool Piece::levelBelowZero() const {
    int lowestLevel = 0;
    for(const Connector& con : this->connectors) {
        if(con.getLevel() < lowestLevel) lowestLevel = con.getLevel();
    }
    return (lowestLevel < 0);
}

int Piece::getLowestLevel() const {
    int lowestLevel = this->connectors.at(0).getLevel();
    for(int i = 1; i < this->connectors.size(); i++) {
        if(this->connectors.at(i).getLevel() < lowestLevel) lowestLevel = this->connectors.at(i).getLevel();
    }
    return lowestLevel;
}
