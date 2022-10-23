#ifndef PIECELIST_H
#define PIECELIST_H

#include "json/json.h"
#include "Piece.h"

class PieceList {
private:
    std::vector<Piece> pieces; // List of pieces
public:
    PieceList(Json::Value::Members selection);
    void addPiece(Piece piece);
};

#endif
