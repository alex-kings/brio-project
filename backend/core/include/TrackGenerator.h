#pragma once
#include "Piece.h"
#include <json/json.h>
#include "ReadJson.h"
#include "Part.h"
#include "Obb.h"
#include "Vec2D.h"
#include <vector>

/**
 * Generates tracks for a given set of pieces.
*/

/**
 * Returns a track generated from the given piece selection. Returns the starting piece of the track.
*/
Piece getTrack(const Json::Value& selection);


/**
 * Takes a start piece, an open piece, a vector of placed pieces and a vector of available pieces. Recursively connects pieces together to generate a closed track.
*/
bool generateTrack(const Piece& startPiece, const Piece& lastPiece, const std::vector<Piece>& placedPieces, const std::vector<Piece>& availablePieces);


/**
 * Returns a vector containing all available pieces from a Json::Value selection.
*/
std::vector<Piece> getAvailablePieces(const Json::Value& selection);
