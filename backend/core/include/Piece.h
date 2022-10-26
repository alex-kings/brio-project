#pragma once
#include "Obb.h"

/**
 * A track piece.
*/

class Piece {
private:
    std::vector<Obb> obbs;
    std::string id;
public:
    Piece(std::string pieceId) {
        id = pieceId;
    }
};
